#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "videoanalyzer.h"
#include "husapp.h"
#include "registerface.h"


int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<FrameAnalyzer>("MyApp", 1, 0, "FrameAnalyzer");
    qmlRegisterType<RegisterFace>("MyApp", 1, 0, "RegisterFace");
    const QUrl url(QStringLiteral("qrc:/fram_qml/qml/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    HusApp::initialize(&engine);
    return app.exec();
}
