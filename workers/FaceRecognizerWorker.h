#pragma once
#include <QPainter>
#include <modeldeploy/vision.h>
#include <opencv2/opencv.hpp>
#include <QVariantMap>
#include "schema/schema.h"
#include <QDir>


class FaceRecognizerWorker : public QObject {
    Q_OBJECT

public:
    explicit FaceRecognizerWorker(QObject* parent = nullptr);

public slots:
    void processFace(const QImage& image, std::vector<modeldeploy::vision::DetectionLandmarkResult>& detResult);
    void saveAttendInfo();

signals:
    void recognizeReady(const QVariantMap&);

private:
    static QString createAttendPicUrl(const QString& staffNo);

    std::vector<std::pair<AttendInfo, QImage>> attendInfos_{};
    std::unique_ptr<modeldeploy::vision::face::SeetaFaceID> face_rec_ = nullptr;
};
