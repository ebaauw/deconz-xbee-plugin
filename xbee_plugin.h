#ifndef XBEE_PLUGIN_H
#define XBEE_PLUGIN_H

#include <list>
#include <QObject>
#include <deconz.h>

class XBeeWidget;

class XBeePluginPrivate;

class XBeePlugin : public QObject,
                   public deCONZ::NodeInterface
{
    Q_OBJECT
    Q_INTERFACES(deCONZ::NodeInterface)
    #if QT_VERSION >= 0x050000
        Q_PLUGIN_METADATA(IID "org.dresden-elektronik.DeXBeePlugin")
    #endif

public:
    explicit XBeePlugin(QObject *parent = 0);
    ~XBeePlugin();
    const char *name();
    bool hasFeature(Features feature);
    QWidget *createWidget();
    QDialog *createDialog();
    void setApsController(deCONZ::ApsController *ctrl);

public Q_SLOTS:

signals:

private:
    XBeePluginPrivate *d;
    XBeeWidget *m_w;
};

#endif // XBEE_PLUGIN_H
