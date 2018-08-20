TEMPLATE        = lib
CONFIG         += plugin \
                  debug_and_release

INCLUDEPATH    += ../.. \
                  ../../common

QMAKE_CXXFLAGS += -Wno-attributes

HEADERS  = xbee_plugin.h \
           xbee_plugin_private.h \
           xbee_widget.h

SOURCES  = xbee_plugin.cpp \
           xbee_plugin_private.cpp \
           xbee_widget.cpp

TARGET   = $$qtLibraryTarget(xbee_plugin)
win32:DESTDIR  = ../../debug/plugins # TODO adjust
unix:DESTDIR  = ..

LIBS+=  -L../.. -ldeCONZ

DEFINES += DECONZ_DLLSPEC=Q_DECL_IMPORT

FORMS += \
    xbee_widget.ui
