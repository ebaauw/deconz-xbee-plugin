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
    DBG_Printf(DBG_INFO, "XBee: plugin loaded\n");
}

XBeePlugin::~XBeePlugin()
{
    delete d;
    d = 0;
}

void XBeePluginPrivate::apsDataIndication(const deCONZ::ApsDataIndication &ind)
{
    DBG_Printf(DBG_INFO, "XBee: apsDataIndication()\n");
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
                DBG_Printf(DBG_INFO, "XBee: AT: %s\n", qPrintable(s));
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
                DBG_Printf(DBG_INFO, "XBee: AT: %s\n", qPrintable(s));
            }
        }
    }
}

void XBeePluginPrivate::apsDataConfirm(const deCONZ::ApsDataConfirm &conf)
{
    DBG_Printf(DBG_INFO, "XBee: apsDataConfirm()\n");
    Q_UNUSED(conf);
    //    qDebug() << Q_FUNC_INFO << conf;
}

int XBeePluginPrivate::apsDataRequestToSelected(deCONZ::ApsDataRequest &req)
{
    DBG_Printf(DBG_INFO, "XBee: apsDataRequestToSelected()\n");

    if (apsCtrl && selected)
    {
        req.dstAddress() = selected->address;
//        if (m_selected->address.hasNwk())
//        {
//            req.setDstAddressMode(deCONZ::ApsNwkAddress);
//        }
//        else
//        {
            req.setDstAddressMode(deCONZ::ApsExtAddress);
//        }
        return apsCtrl->apsdeDataRequest(req);
    }

    return -1;
}

void XBeePluginPrivate::nodeEvent(const deCONZ::NodeEvent &event)
{
    switch (event.event())
    {
    case deCONZ::NodeEvent::NodeAdded:
    {
        addIfUnknown(event.node());
    }
        break;

    case deCONZ::NodeEvent::NodeSelected:
    {

        XBee *x = addIfUnknown(event.node());
        // x might be 0
        selected = x;
        if (x != 0)
        {
            DBG_Printf(DBG_INFO, "XBee: %s selected\n", qPrintable(event.node()->address().toStringExt()));
        }
        else
        {
            DBG_Printf(DBG_INFO, "XBee: no XBee selected\n");
        }
    }
        break;

    case deCONZ::NodeEvent::NodeDeselected:
        DBG_Printf(DBG_INFO, "XBee: no XBee selected\n");
        selected = 0;
        break;

    default:
        // DBG_Printf(DBG_INFO, "XBee: unhandled node event %d\n", event);
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
