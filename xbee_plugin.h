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

public:
    explicit XBeePlugin(QObject *parent = 0);
    ~XBeePlugin();
    const char *name();
    bool hasFeature(Features feature);
    QWidget *createWidget();
    QDialog *createDialog();
    void setApsController(deCONZ::ApsController *ctrl);

public Q_SLOTS:
    void apsDataIndication(const deCONZ::ApsDataIndication &ind);
    void apsDataConfirm(const deCONZ::ApsDataConfirm &conf);
    int apsDataRequestToSelected(deCONZ::ApsDataRequest &req);
    void nodeEvent(int event, const deCONZ::Node *node);

signals:

private:
    XBeePluginPrivate *d;
    XBeeWidget *m_w;
};

#endif // XBEE_PLUGIN_H
