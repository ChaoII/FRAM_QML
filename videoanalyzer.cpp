#pragma once


#include "qmlimageutils.h"
#include "videoanalyzer.h"
#include "core/ConfigManager.h"


FrameAnalyzer::FrameAnalyzer(QObject* parent) : QObject(parent) {
    // 人脸检测线程
    faceDetWorkerThread_ = new QThread(this);
    faceDetWorker_ = new FaceDetectionWorker();
    faceDetWorker_->moveToThread(faceDetWorkerThread_);
    connect(this, &FrameAnalyzer::frameCaptured, faceDetWorker_,
            &FaceDetectionWorker::processFrame);
    connect(faceDetWorker_, &FaceDetectionWorker::detectReady,
            [this](const std::vector<modeldeploy::vision::DetectionLandmarkResult>
            & detRests) {
                auto result = convertToVariantList(detRests);
                auto detRes = detRests;
                emit faceCropReady(detRes);
                emit detectionRect(result);
                busy_ = false;
            });
    faceDetWorkerThread_->start();
    // 人脸识别线程
    faceRecWorkerThread_ = new QThread(this);
    faceRecWorker_ = new FaceRecognizerWorker();
    faceRecWorker_->moveToThread(faceRecWorkerThread_);
    connect(
        this, &FrameAnalyzer::faceCropReady,
        [&](std::vector<modeldeploy::vision::DetectionLandmarkResult>& detRes) {
            faceRecWorker_->processFace(lastFrame_, detRes);
        });
    connect(faceRecWorker_, &FaceRecognizerWorker::recognizeReady, this, &FrameAnalyzer::recognitionResult);
    faceRecWorkerThread_->start();

    // 定期保存打卡记录定时器(去重)
    timer_ = new QTimer(this);
    // 使用配置文件
    timer_->setInterval(ConfigManager::instance()->attendCacheInterval());
    connect(timer_, &QTimer::timeout, faceRecWorker_, &FaceRecognizerWorker::saveAttendInfo);
    timer_->start();
}


FrameAnalyzer::~FrameAnalyzer() {
    if (faceDetWorkerThread_) {
        faceDetWorkerThread_->quit();
        faceDetWorkerThread_->wait();
    }
    if (faceRecWorkerThread_) {
        faceRecWorkerThread_->quit();
        faceRecWorkerThread_->wait();
    }
    if (sink_) {
        disconnect(sink_, nullptr, this, nullptr);
    }
}


QVideoSink* FrameAnalyzer::videoSink() const { return sink_; }

QSize FrameAnalyzer::dstSize() const { return dstSize_; }

void FrameAnalyzer::setDstSize(QSize size) {
    if (size != dstSize_) {
        dstSize_ = size;
        emit dstSizeChanged();
    }
}

QVariantList FrameAnalyzer::convertToVariantList(
    const std::vector<modeldeploy::vision::DetectionLandmarkResult>& dets) {
    QVariantList list;
    for (const auto& r : dets) {
        QVariantMap map;
        map["x"] = r.box.x;
        map["y"] = r.box.y;
        map["width"] = r.box.width;
        map["height"] = r.box.height;
        map["score"] = r.score;
        map["label_id"] = r.label_id;
        // 转换 landmarks
        QVariantList lmList;
        lmList.reserve(static_cast<int>(r.landmarks.size()));
        for (const auto& pt : r.landmarks) {
            QVariantMap lm;
            lm["x"] = pt.x;
            lm["y"] = pt.y;
            lmList.append(lm);
        }
        map["landmarks"] = lmList;
        list.append(map);
    }
    return list;
}

void FrameAnalyzer::setVideoSink(QVideoSink* sink) {
    if (sink_ == sink)
        return;
    if (sink_) {
        disconnect(sink_, nullptr, this, nullptr);
        sink_ = nullptr;
    }
    sink_ = sink;
    if (sink_) {
        connect(sink_, &QVideoSink::videoFrameChanged, this,
                &FrameAnalyzer::onNewFrame);
    }
    emit videoSinkChanged();
}

void FrameAnalyzer::onNewFrame(const QVideoFrame& frame) {
    // 把busy_ 改为true并且返回原来的值，也就是在线程中未完成没有将busy_改为true
    // 那么就直接返回，如果线程完成了busy_的值就是false，这里会返回false然后将busy_
    // 设置为true
    if (busy_.exchange(true)) {
        // 上一帧还在处理中，丢弃本帧
        return;
    }
    if (!frame.isValid())
        return;
    // frame.map(QVideoFrame::ReadOnly);
    const QImage originalImage = frame.toImage();
    // frame.unmap();

    // 裁剪到 dstSize_
    const QImage croppedImage = QmlImageUtils::cropImage(originalImage, dstSize_);
    lastFrame_ = croppedImage;
    emit frameCaptured(croppedImage);
    qDebug() << "Got frame:" << croppedImage.size();
}
