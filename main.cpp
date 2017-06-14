#include <QImage>
#include <QDebug>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>


#include "videoplayer.h"


int main(int argc, char ** argv)
{
#ifdef Q_PROCESSOR_ARM
    //设置可触摸
    setenv("QT_QPA_GENERIC_PLUGINS", "evdevtouch:/dev/input/event0", 1);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<VideoPlayer>("VideoPlayer", 1, 0, "VideoPlayer");

    QQuickView viewer;
#ifdef Q_PROCESSOR_ARM
    viewer.setSource(QUrl("qrc:///main.qml"));
#else
    viewer.setSource(QUrl("qrc:///main-pc.qml"));
#endif
    viewer.show();

    return app.exec();
}
