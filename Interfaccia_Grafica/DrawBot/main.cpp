#include <QGuiApplication>
#include <QQmlApplicationEngine>

// user includes
#include "structpath.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // user code begin
    QCoreApplication::setOrganizationName("Pietro");
    QCoreApplication::setApplicationName("DrawBot");
    // end user code

    QGuiApplication app(argc, argv);

    // user code begin
    qmlRegisterType< Structpath > ("stringCommands", 1, 0, "DashPath");
    // end uder code

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
