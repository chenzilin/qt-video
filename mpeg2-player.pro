# QT Modules
QT += qml quick concurrent widgets

# Target
TARGET = mpeg2_player
TEMPLATE = app

CONFIG += c++14
CONFIG += link_pkgconfig
PKGCONFIG += libmpeg2 libmpeg2convert

# Extra Configs
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

# Sources and Headers
HEADERS += \
    mpeg2dec.h \
    mpeg2player.h

SOURCES += \
    main.cpp \
    mpeg2dec.cpp \
    mpeg2player.cpp

# Qml Files
OTHER_FILES += main.qml

# Resources
RESOURCES += qml.qrc
QMAKE_RESOURCE_FLAGS += -no-compress

# Objects Pathes
DESTDIR = bin
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
OBJECTS_DIR = build
