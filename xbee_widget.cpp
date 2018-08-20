#include <QDebug>
#include "xbee_plugin.h"
#include "xbee_plugin_private.h"
#include "xbee_widget.h"
#include "ui_xbee_widget.h"

XBeeWidget::XBeeWidget(QWidget *parent, XBeePlugin *plugin, XBeePluginPrivate *priv) :
    QWidget(parent),
    ui(new Ui::XBeeWidget)
{
    d = priv;
    m_plugin = plugin;
    ui->setupUi(this);
    createUi();
}

XBeeWidget::~XBeeWidget()
{
    d = 0;
    delete ui;
}

void XBeeWidget::atCommandSendClicked()
{
    QString cmd = ui->atCmdEdit->text().toUpper();
    d->sendAtCommand(cmd, ui->atCmdParamEdit->text());
}

void XBeeWidget::showNode(XBee *x)
{
    m_xbee = x;
}

void XBeeWidget::createUi()
{
    connect(ui->atCmdSendButton, SIGNAL(clicked()),
            this, SLOT(atCommandSendClicked()));

}
