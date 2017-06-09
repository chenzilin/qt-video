#include <QImage>
#include <QDebug>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>


#include "mpeg2player.h"


int main(int argc, char ** argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Mpeg2Player>("Mpeg2Player", 1, 0, "Mpeg2Player");

    QQuickView viewer;
    viewer.setSource(QUrl("qrc:///main.qml"));
    viewer.show();

    return app.exec();
}
