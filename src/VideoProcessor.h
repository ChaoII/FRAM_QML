//
// Created by aichao on 2025/9/30.
//

#pragma once


#include <QObject>
#include <QThread>

#include "providers/CameraImageProvider.h"
#include "src/workers/CameraDecodeWorker.h"
#include "src/workers/FaceDetectionWorker.h"
#include "src/ThreadSafeQueue.h"

class VideoProcessor : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool processing READ isProcessing NOTIFY processingChanged)

public:
    enum class CameraSourceType {
        Local,
        RTSP,
        File
    };

    Q_ENUM(CameraSourceType)

    explicit VideoProcessor(const QString& source, CameraSourceType type, QObject* parent = nullptr);
    ~VideoProcessor();
    [[nodiscard]] CameraImageProvider* imageProvider() const { return provider_; }
    Q_INVOKABLE void startProcessing();
    Q_INVOKABLE void stopProcessing();
    bool isProcessing() const { return processing_; }

signals:
    void frameReady();
    void detectionReady(const QVariantList& results);
    void errorOccurred(const QString& error);
    void processingChanged();

private:
    QThread captureThread_;
    QThread inferenceThread_;
    CameraDecodeWorker* captureWorker_;
    FaceDetectionWorker* inferenceWorker_;
    CameraImageProvider* provider_;
    ThreadSafeQueue<cv::Mat> frameQueue_;

    bool processing_{false};
};
