//
// Created by aichao on 2025/9/30.
//


#include "src/VideoProcessor.h"
#include "src/workers/CameraDecodeWorker.h"
#include "src/workers/FaceDetectionWorker.h"

VideoProcessor::VideoProcessor(const QString& source, CameraSourceType type, QObject* parent)
    : QObject(parent) {
    provider_ = new CameraImageProvider();

    // 创建工作者对象
    captureWorker_ = new CameraDecodeWorker();

    if (type == CameraSourceType::File || type == CameraSourceType::RTSP) {
        captureWorker_->InitMediaDevice(source);
    }
    else if (type == CameraSourceType::Local) {
        captureWorker_->InitMediaDevice(source.toInt());
    }


    inferenceWorker_ = new FaceDetectionWorker();

    // 设置队列
    captureWorker_->setFrameQueue(&frameQueue_);
    inferenceWorker_->setFrameQueue(&frameQueue_);

    // 移动到线程
    captureWorker_->moveToThread(&captureThread_);
    inferenceWorker_->moveToThread(&inferenceThread_);

    // 连接信号槽


    connect(&captureThread_, &QThread::started, captureWorker_, &CameraDecodeWorker::startCameraDecode);
    connect(&inferenceThread_, &QThread::started, inferenceWorker_, &FaceDetectionWorker::startFaceDetection);

    connect(captureWorker_, &CameraDecodeWorker::decodeFinished,
            this, [this](const QImage& image) {
                provider_->setImage(image);
                emit frameReady();
            });
    connect(captureWorker_, &CameraDecodeWorker::captureError,
            this, [this](const QString& info) {
                qDebug() << "capture error:" << info;
            });

    connect(inferenceWorker_, &FaceDetectionWorker::detectionFinished,
            this, &VideoProcessor::detectionReady);
    connect(inferenceWorker_, &FaceDetectionWorker::inferenceError,
            this, [this](const QString& info) {
                qDebug() << "capture error:" << info;
            });

    // 启动线程
    captureThread_.start();
    inferenceThread_.start();
}

VideoProcessor::~VideoProcessor() {
    stopProcessing();
    captureThread_.quit();
    inferenceThread_.quit();
    captureThread_.wait();
    inferenceThread_.wait();
    delete captureWorker_;
    delete inferenceWorker_;
}

void VideoProcessor::startProcessing() {
    if (processing_) return;
    processing_ = true;
    emit processingChanged();
}

void VideoProcessor::stopProcessing() {
    if (!processing_) return;
    processing_ = false;
    emit processingChanged();
    frameQueue_.shutdown();
    // resultQueue_.shutdown();
}
