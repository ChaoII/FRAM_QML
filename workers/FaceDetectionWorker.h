#pragma once
#include <QPainter>
#include "modeldeploy/vision.h"
#include <opencv2/opencv.hpp>
#include <QVariantMap>


class FaceDetectionWorker : public QObject {
    Q_OBJECT

public:
    explicit FaceDetectionWorker(QObject* parent = nullptr);

public slots:
    void processFrame(const QImage& image);


signals:
    void detectReady(const std::vector<modeldeploy::vision::DetectionLandmarkResult>& detResult);

private:
    std::unique_ptr<modeldeploy::vision::face::Scrfd> face_det_ = nullptr;
};
