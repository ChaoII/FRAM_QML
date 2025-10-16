#pragma once
#include "workers/FaceDetectionWorker.h"
#include "workers/FaceRecognizerWorker.h"
#include <QImage>
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QVideoFrame>
#include <QVideoSink>

class FrameAnalyzer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVideoSink *videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    Q_PROPERTY(QSize dstSize READ dstSize WRITE setDstSize NOTIFY dstSizeChanged)

public:
    explicit FrameAnalyzer(QObject* parent = nullptr);

    ~FrameAnalyzer() override;

    QVideoSink* videoSink() const;

    QSize dstSize() const;

    void setDstSize(QSize size);

    static QVariantList convertToVariantList(
        const std::vector<modeldeploy::vision::DetectionLandmarkResult>& dets);

    void setVideoSink(QVideoSink* sink);

signals:
    void videoSinkChanged();

    void dstSizeChanged();

    void frameCaptured(const QImage&);

    void faceCropReady(std::vector<modeldeploy::vision::DetectionLandmarkResult>&);

    void detectionRect(const QVariantList& detResults);

    void recognitionResult(const QVariantMap&);

private slots:
    void onNewFrame(const QVideoFrame& frame);

private:
    QTimer* timer_ = nullptr;

    QVideoSink* sink_ = nullptr;
    QSize dstSize_ = QSize(320, 600);
    QThread* faceDetWorkerThread_;
    FaceDetectionWorker* faceDetWorker_;
    QThread* faceRecWorkerThread_;
    FaceRecognizerWorker* faceRecWorker_;
    QImage lastFrame_;
    std::atomic_bool busy_ = false;
};
