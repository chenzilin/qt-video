# QT Modules
QT += qml quick

# Target
TARGET = video_player
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
    videoplayer.h

SOURCES += \
    main.cpp \
    mpeg2dec.cpp \
    videoplayer.cpp

# Qml Files
OTHER_FILES += \
    main.qml \
    main-pc.qml

# Resources
RESOURCES += qml.qrc
QMAKE_RESOURCE_FLAGS += -no-compress

# Objects Pathes
DESTDIR = bin
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
OBJECTS_DIR = build
