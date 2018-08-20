#ifndef XBEE_WIDGET_H
#define XBEE_WIDGET_H

#include <QWidget>

namespace Ui {
class XBeeWidget;
}

class XBeePlugin;
class XBeePluginPrivate;
struct XBee;

class XBeeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit XBeeWidget(QWidget *parent, XBeePlugin *plugin, XBeePluginPrivate *priv);
    ~XBeeWidget();

public Q_SLOTS:
    void atCommandSendClicked();
    void showNode(XBee *x);
    
private:
    void createUi();
    Ui::XBeeWidget *ui;
    XBeePlugin *m_plugin;
    XBeePluginPrivate *d;
    XBee *m_xbee;
};

#endif // XBEE_WIDGET_H
