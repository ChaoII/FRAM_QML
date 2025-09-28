//
// Created by aichao on 2025/9/28.
//

#include "src/CameraDecoder.h"
#include <QDebug>

CameraDecoder::CameraDecoder(const QString& source, CameraSourceType type, QObject* parent): QObject(parent) {
    worker_ = new CameraDecodeWorker();
    if (type == CameraSourceType::File || type == CameraSourceType::RTSP) {
        worker_->InitMediaDevice(source);
    }
    else if (type == CameraSourceType::Local) {
        worker_->InitMediaDevice(source.toInt());
    }
    workerThread_ = new QThread(this);
    worker_->moveToThread(workerThread_);

    provider_ = new CameraImageProvider();
    connect(workerThread_, &QThread::started, worker_, &CameraDecodeWorker::startCameraDecode);
    connect(worker_, &CameraDecodeWorker::decodeFinished, this, &CameraDecoder::handleImage, Qt::QueuedConnection);
}


void CameraDecoder::startDecode() {
    qDebug() << "start decode";
    workerThread_->start();
}

void CameraDecoder::stopDecode() {
    worker_->stopTask();
    workerThread_->quit();
    workerThread_->wait();
}

CameraDecoder::~CameraDecoder() {
    worker_->stopTask();
    workerThread_->quit();
    workerThread_->wait();
}

void CameraDecoder::handleImage(const QImage& img) {
    qDebug() << "handleImage called, image size:" << img.size() << img.isNull();
    provider_->setImage(img);
    emit imageReady();
}
