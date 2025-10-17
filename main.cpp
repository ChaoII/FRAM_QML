#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "videoanalyzer.h"
#include "husapp.h"
#include "registerface.h"
#include "core/ConfigManager.h"
#include "task/ClearOldAttendInfoTask.h"


int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    ClearOldAttendInfoTask::start();
    qmlRegisterType<FrameAnalyzer>("MyApp", 1, 0, "FrameAnalyzer");
    qmlRegisterType<RegisterFace>("MyApp", 1, 0, "RegisterFace");
    qmlRegisterSingletonType<ConfigManager>("MyApp", 1, 0, "ConfigManager", ConfigManager::create);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // const auto url = QUrl(QStringLiteral("qrc:/Fram/qml/Main.qml"));
    // engine.load(url);

    engine.loadFromModule("Fram", "Main");
    HusApp::initialize(&engine);
    return app.exec();
}
