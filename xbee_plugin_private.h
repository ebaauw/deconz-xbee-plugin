#ifndef XBEE_PLUGIN_PRIVATE_H
#define XBEE_PLUGIN_PRIVATE_H

#include <list>
#include <deconz.h>

#define DIGI_PROFILE_ID 0xC105 //
#define AT_MAX_NAME_LENGTH 16
#define AT_MAX_DESCRIPTION_LENGTH 16

enum XBeeAtId
{
    XBeeAt_DH = 0x0000,
    XBeeAt_DL,
    XBeeAt_MY,
    XBeeAt_MP,
    XBeeAt_SH,
    XBeeAt_SL,
    XBeeAt_NI
};

enum XBeeAtSection
{
    XBeeSectionNetworking,
    XBeeSectionAddressing,
    XBeeSectionRfInterfacing,
    XBeeSectionSecurity,
    XBeeSectionSerialInterfacing,
    XBeeSectionSleepModes,
    XBeeSectionIoSettings,
    XBeeSectionDiagnostic
};

enum XBeeDatatype
{
    XBeeBool,
    XBeeUint8,
    XBeeUint16,
    XBeeUint24,
    XBeeUint32,
    XBeeUint64,
    XBeeCString
};

struct XBeeAtInfo
{
    XBeeAtId cmd;
    XBeeDatatype type;
    const char *name;
    const char *description;
};

struct XBeeAtCommand
{
    const XBeeAtInfo *info;

    union
    {
        uint8_t u8;
        uint8_t u16;
        uint8_t u32;
        uint8_t u64;
        char data[32];
    } param;

    char param_readable[32];
};

struct XBee
{
    deCONZ::Address address;
    std::list<XBeeAtCommand> atCommands;
};

class XBeePluginPrivate : public QObject
{
    Q_OBJECT

public:
    XBeePluginPrivate();
    int sendAtCommand(const QString &cmd, const QString &param);
    XBee *addIfUnknown(const deCONZ::Node *node);
    void initAtCommands(std::list<XBeeAtCommand> &ls);
    const XBee *getSelected();

public Q_SLOTS:
    void timer0Fired();

public:
    std::list<XBee> xbees;
    deCONZ::ApsController *apsCtrl;
    XBee *selected;
    QTimer *timer0;
};

#endif // XBEE_PLUGIN_PRIVATE_H
