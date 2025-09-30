#include <QQuickWindow>
#include <QQmlContext>

#include "src/CameraManager.h"
#include "husapp.h"
#include "LocalCamera.h"


int main(int argc, char* argv[]) {
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QGuiApplication app(argc, argv);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QQuickWindow::setDefaultAlphaBuffer(true);

    qRegisterMetaType<DetectionResults>("DetectionResults");

    QQmlApplicationEngine engine;
    CameraManager manager(&engine);
    LocalCamera localCamera;
    engine.rootContext()->setContextProperty("cameraManager", &manager);
    engine.rootContext()->setContextProperty("localCamera", &localCamera);
    qmlRegisterUncreatableType<VideoProcessor>("MyApp", 1, 0, "VideoProcessor", "Enum only");


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
