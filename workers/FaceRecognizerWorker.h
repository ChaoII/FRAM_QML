#pragma once
#include <QPainter>
#include "modeldeploy/vision.h"
#include <opencv2/opencv.hpp>
#include <QVariantMap>


class FaceRecognizerWorker : public QObject {
    Q_OBJECT

public:
    explicit FaceRecognizerWorker(QObject* parent = nullptr);

public slots:
    void processFace(const QImage& image, std::vector<modeldeploy::vision::DetectionLandmarkResult>& detResult);


signals:
    void recognizeReady(const QVariantMap&);

private:
    std::unique_ptr<modeldeploy::vision::face::SeetaFaceID> face_rec_ = nullptr;
};
