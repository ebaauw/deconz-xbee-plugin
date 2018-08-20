#include <QTimer>
#include "deconz/dbg_trace.h"
#include "xbee_plugin_private.h"

/*

http://jdesbonnet.blogspot.de/2011/09/controlling-xbee-io-lines-with-zigbee.html

XBee private profile

Profile ID: 0xC105

Endpoint 0xE6 - DDO Digi Device Object

Cluster   Name
----------------------------------
0x0021   Remote AT commands request
0x00a1   Remote AT command response



Endpoint 0xE8 - DD Digi Data

Cluster  Name
--------------------------
0x0011   Data Transmit
0x0012   Loopback


From XBee_ZB_90000976_F.pdf

Use the transmit request, or explicit transmit request frame (0x10 and 0x11 respectively) to send data to
the coordinator. The 64-bit address can either be set to 0x0000000000000000, or to the 64-bit address of
the coordinator. The 16-bit address should be set to 0xFFFE when using the 64-bit address of all 0x00s.
To send an ascii "1" to the coordinator's 0x00 address, the following API frame can be used:
7E 00 0F 10 01 0000 0000 0000 0000 FFFE 00 00 31 C0
If the explicit transmit frame is used, the cluster ID should be set to 0x0011, the profile ID to 0xC105, and
the source and destination endpoints to 0xE8 (recommended defaults for data transmissions in the Digi
profile.) The same transmission could be sent using the following explicit transmit frame:
7E 00 15 11 01 0000 0000 0000 0000 FFFE E8 E8 0011 C105 00 00 31 18
Notice the 16-bit address is set to 0xFFFE. This is required when sending to a 64-bit address of 0x00s.
Now suppose the coordinator's 64-bit address is 0x0013A200404A2244. The following transmit request API
frame (0x10) will send an ASCII "1" to the coordinator:
7E 00 0F 10 01 0013 A200 404A 2244 0000 0000 31 18

*/

static const XBeeAtInfo niAtInfo = {XBeeAt_DH, XBeeCString, "Node Identifier", "No description." };
static const XBeeAtInfo dhAtInfo = {XBeeAt_DH, XBeeUint32, "Destination High", "No description." };
static const XBeeAtInfo dlAtInfo = {XBeeAt_DL, XBeeUint32, "Destination Low", "No description." };

static XBeeAtCommand createCommand(const XBeeAtInfo *info)
{
    XBeeAtCommand cmd;

    cmd.info = info;
    memset(cmd.param_readable, 0, sizeof(cmd.param_readable));
    memset(&cmd.param, 0, sizeof(cmd.param));

    return cmd;
}

XBeePluginPrivate::XBeePluginPrivate() :
    QObject(0)
{
    apsCtrl = 0;
    selected = 0;

    timer0 = new QTimer(this);
    timer0->setSingleShot(false);
    connect(timer0, SIGNAL(timeout()),
            this, SLOT(timer0Fired()));
    timer0->start(3000);
}

/*!
   Send a remote AT command to the selected XBee.

   \return 0 - on success
          -1 - if nothing was send
 */
int XBeePluginPrivate::sendAtCommand(const QString &cmd, const QString &param)
{
    if (!apsCtrl || !selected)
    {
        return -1;
    }

    deCONZ::Node *self = 0;

    if ((apsCtrl->getNode(0, &self) != 0) || (self == 0))
    {
        return -1;
    }

    if (cmd.isEmpty() || (cmd.size() > 2))
    {
        return -1;
    }

    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
//    stream.setByteOrder(QDataStream::LittleEndian);

    stream << (uint8_t)0x00;
    stream << (uint8_t)0x32;
    stream << (uint8_t)0x00;
    stream << (uint8_t)0x0f; // frame-id
//    stream << (uint8_t)0x02; // ZigBee Stack Profile?

//    stream << (uint8_t)0x00; // sender mac0
//    stream << (uint8_t)0x21; // sender mac1
//    stream << (uint8_t)0x2e; // sender mac2
//    stream << (uint8_t)0xff; // sender mac3
//    stream << (uint8_t)0xff; // sender mac4
//    stream << (uint8_t)0x00; // sender mac5
//    stream << (uint8_t)0x03; // sender mac6
//    stream << (uint8_t)0x52; // sender mac7

    stream << self->address().ext();
    stream << self->address().nwk();
//    stream << (uint8_t)0x00; //
//    stream << (uint8_t)0x00; //

    stream << (uint8_t)cmd.at(0).toAscii();
    if (cmd.size() == 2)
    {
        stream << (uint8_t)cmd.at(1).toAscii();
    }

    deCONZ::ApsDataRequest req;
    req.setProfileId(DIGI_PROFILE_ID);
    req.setClusterId(0x0021); // remote AT command request
    req.setSrcEndpoint(0xE6);
    req.setDstEndpoint(0xE6);
    req.setTxOptions(deCONZ::ApsTxAcknowledgedTransmission);
    req.setAsdu(arr);
    req.setRadius(0);

    req.setDstAddressMode(deCONZ::ApsExtAddress);
    req.dstAddress() = selected->address;

    if (apsCtrl->apsdeDataRequest(req) == 0)
    {
        return 0;
    }

    return -1;
}

/*!
    Checks if \p node is a XBee and adds it to cache if not known.

    \return XBee pointer in cache - if added or already known
            0 - if \p node is no XBee
 */
XBee *XBeePluginPrivate::addIfUnknown(const deCONZ::Node *node)
{
    if (!node)
    {
        return 0;
    }

    deCONZ::SimpleDescriptor sd;

    // check for digi endpoint
    if (node->copySimpleDescriptor(0xE6, &sd) != 0)
    {
        return 0;
    }

    if (sd.profileId() != DIGI_PROFILE_ID)
    {
        return 0;
    }

    // check if exist
    std::list<XBee>::iterator i = xbees.begin();
    std::list<XBee>::iterator end = xbees.end();

    for ( ;i != end; ++i)
    {
        if (node->address().ext() == i->address.ext())
        {
            return &(*i);
        }
    }

    // create new
    DBG_Printf(DBG_INFO, "Added new XBee %s\n", qPrintable(node->address().toStringExt()));
    XBee x;
    x.address = node->address();
    xbees.push_back(x);
    return &xbees.back();
}

void XBeePluginPrivate::initAtCommands(std::list<XBeeAtCommand> &ls)
{
    ls.clear();
    ls.push_back(createCommand(&dhAtInfo));
    ls.push_back(createCommand(&dlAtInfo));
    ls.push_back(createCommand(&niAtInfo));
}

const XBee *XBeePluginPrivate::getSelected()
{
    return selected;
}

void XBeePluginPrivate::timer0Fired()
{
    if (selected)
    {
        QByteArray arr;
        static uint8_t cnt  = 0;
        if (cnt == 10) cnt = 0;
        arr.append((char)(cnt + 0x30));
        cnt++;
        arr.append('A');
        arr.append('B');
        arr.append('C');

        deCONZ::ApsDataRequest req;
        req.setProfileId(DIGI_PROFILE_ID);
        req.setClusterId(0x0011); // Transmit
        req.setSrcEndpoint(0xE8);
        req.setDstEndpoint(0xE8);
//        req.setTxOptions(deCONZ::ApsTxAcknowledgedTransmission);
        req.setAsdu(arr);
        req.setRadius(0);


        req.setDstAddressMode(deCONZ::ApsExtAddress);
        req.dstAddress() = selected->address;

        if (apsCtrl->apsdeDataRequest(req) == 0)
        {
            return;
        }
    }
}
