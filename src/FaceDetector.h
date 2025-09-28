#pragma once
#include <opencv2/highgui.hpp>

#include "providers/CameraImageProvider.h"
#include "src/workers/FaceDetectionWorker.h"

class FaceDetector : public QObject {
    Q_OBJECT

public:
    explicit FaceDetector(QObject* parent = nullptr)
        : QObject(parent) {
        m_worker = new FaceDetectionWorker();
        m_workerThread = new QThread(this);
        m_worker->moveToThread(m_workerThread);

        connect(m_workerThread, &QThread::started, m_worker, &FaceDetectionWorker::startFaceDetection);
        connect(m_worker, &FaceDetectionWorker::detectionFinished, this, &FaceDetector::handleImage);
        m_workerThread->start();

        m_provider = new CameraImageProvider();
    }

    ~FaceDetector() override {
        m_worker->stopTask();
        m_workerThread->quit();
        m_workerThread->wait();
        delete m_worker;
    }

    CameraImageProvider* imageProvider() const { return m_provider; }

public slots:
    void startDetection() { m_workerThread->start(); }

    void stopDetection() {
        m_worker->stopTask();
        m_workerThread->quit();
        m_workerThread->wait();
    }

signals:
    void imageReady();

private slots:
    void handleImage(const QImage& img) {


        m_provider->setImage(img);
        emit imageReady();
    }

private:
    FaceDetectionWorker* m_worker;
    QThread* m_workerThread;
    CameraImageProvider* m_provider;
};
