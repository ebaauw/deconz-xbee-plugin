#include <QDebug>
#include <QtPlugin>
#include <QPushButton>
#include "xbee_plugin.h"
#include "xbee_plugin_private.h"
#include "xbee_widget.h"

XBeePlugin::XBeePlugin(QObject *parent) :
    QObject(parent)
{
    d = new XBeePluginPrivate;
    m_w = 0;
}

XBeePlugin::~XBeePlugin()
{
    delete d;
    d = 0;
}

void XBeePlugin::apsDataIndication(const deCONZ::ApsDataIndication &ind)
{
    if (ind.profileId() == DIGI_PROFILE_ID)
    {
        if (ind.clusterId() == 0x00a1)
        {
            QString s;
            // [0] is frame id
            for (int i = 1; i < ind.asdu().size(); i++)
            {
                char c = ind.asdu().at(i);

                if (isalnum(c))
                {
                    s.append(c);
                }
                else if (c == 0x00 && i == 3) // seperator
                {
                    s.append(' ');
                }
                else
                {
                    QString hex;
                    hex.sprintf(" %02X", c & 0xFF);
                    s.append(hex);
                }
            }

            if (!s.isEmpty())
            {
                DBG_Printf(DBG_INFO, "AT: %s\n", qPrintable(s));
            }
        }
    }

    if (ind.profileId() == DIGI_PROFILE_ID)
    {
        if (ind.clusterId() == 0x00a1)
        {
            QString s;
            // [0] is frame id
            for (int i = 1; i < ind.asdu().size(); i++)
            {
                char c = ind.asdu().at(i);

                if (isalnum(c))
                {
                    s.append(c);
                }
                else if (c == 0x00 && i == 3) // seperator
                {
                    s.append(' ');
                }
                else
                {
                    QString hex;
                    hex.sprintf(" %02X", c & 0xFF);
                    s.append(hex);
                }
            }

            if (!s.isEmpty())
            {
                DBG_Printf(DBG_INFO, "AT: %s\n", qPrintable(s));
            }
        }
    }
}

void XBeePlugin::apsDataConfirm(const deCONZ::ApsDataConfirm &conf)
{
    Q_UNUSED(conf);
    //    qDebug() << Q_FUNC_INFO << conf;
}

int XBeePlugin::apsDataRequestToSelected(deCONZ::ApsDataRequest &req)
{
    if (d->apsCtrl && d->selected)
    {
        req.dstAddress() = d->selected->address;
//        if (m_selected->address.hasNwk())
//        {
//            req.setDstAddressMode(deCONZ::ApsNwkAddress);
//        }
//        else
//        {
            req.setDstAddressMode(deCONZ::ApsExtAddress);
//        }
        return d->apsCtrl->apsdeDataRequest(req);
    }

    return -1;
}

void XBeePlugin::nodeEvent(int event, const deCONZ::Node *node)
{

    switch (event)
    {
    case deCONZ::NodeEvent::NodeAdded:
    {
        d->addIfUnknown(node);
    }
        break;

    case deCONZ::NodeEvent::NodeSelected:
    {
        XBee *x = d->addIfUnknown(node);
        // x might be 0
        d->selected = x;
    }
        break;


    case deCONZ::NodeEvent::NodeDeselected:
        d->selected = 0;
        break;

    default:
        DBG_Printf(DBG_INFO, "XBee unhandled node event %d\n", event);
        break;
    }
}

bool XBeePlugin::hasFeature(Features feature)
{
    switch (feature)
    {
    case WidgetFeature:
        return true;

    default:
        break;
    }

    return false;
}

QWidget *XBeePlugin::createWidget()
{
    if (!m_w)
    {
        m_w = new XBeeWidget(0, this, d);
    }

    return m_w;
}

QDialog *XBeePlugin::createDialog()
{
    return 0;
}

void XBeePlugin::setApsController(deCONZ::ApsController *ctrl)
{
    d->apsCtrl = ctrl;
}

const char *XBeePlugin::name()
{
    return "XBee Plugin";
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(xbee_plugin, XBeePlugin)
#endif
