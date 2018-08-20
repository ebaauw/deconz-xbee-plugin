TARGET = de_xbee_plugin

# common configuration for deCONZ plugins

TARGET = $$qtLibraryTarget($$TARGET)

DEFINES += DECONZ_DLLSPEC=Q_DECL_IMPORT

unix:contains(QMAKE_HOST.arch, armv6l) {
    DEFINES += ARCH_ARM ARCH_ARMV6
}
unix:contains(QMAKE_HOST.arch, armv7l) {
    DEFINES += ARCH_ARM ARCH_ARMV7
}

QMAKE_CXXFLAGS += -Wno-attributes \
                  -Wall

CONFIG(debug, debug|release) {
    LIBS += -L../../debug
}

CONFIG(release, debug|release) {
    LIBS += -L../../release
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += core gui widgets serialport

    greaterThan(QT_MINOR_VERSION, 2) {
        DEFINES += USE_WEBSOCKETS
        QT += websockets
    }
}

QMAKE_SPEC_T = $$[QMAKE_SPEC]

contains(QMAKE_SPEC_T,.*linux.*) {
    CONFIG += link_pkgconfig
    packagesExist(sqlite3) {
        DEFINES += HAS_SQLITE3
        PKGCONFIG += sqlite3
    }
}

unix:LIBS +=  -L../.. -ldeCONZ

unix:!macx {
    LIBS += -lcrypt
}

TEMPLATE        = lib
CONFIG         += plugin \
               += debug_and_release \
               += c++11

INCLUDEPATH    += ../.. \
                  ../../common

HEADERS  = xbee_plugin.h \
           xbee_plugin_private.h \
           xbee_widget.h

SOURCES  = xbee_plugin.cpp \
           xbee_plugin_private.cpp \
           xbee_widget.cpp

win32:DESTDIR  = ../../debug/plugins # TODO adjust
unix:DESTDIR  = ..

FORMS += \
    xbee_widget.ui
