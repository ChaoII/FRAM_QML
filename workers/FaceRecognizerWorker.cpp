//
// Created by aichao on 2025/9/25.
//

#include "workers/FaceRecognizerWorker.h"
#include <QRandomGenerator>
#include <QTimer>
#include <QThread>
#include "utils.h"

FaceRecognizerWorker::FaceRecognizerWorker(QObject* parent): QObject(parent) {
    face_rec_ = std::make_unique<modeldeploy::vision::face::SeetaFaceID>(
        "E:/CLionProjects/ModelDeploy/test_data/test_models/face/face_recognizer.onnx");
}

void FaceRecognizerWorker::processFace(const QImage& image,
                                       std::vector<modeldeploy::vision::DetectionLandmarkResult>& detResult) {
    if (detResult.empty()) return;
    auto mat = Utils::QImageToCvMat(image);
    if (mat.empty()) {
        return;
    }
    cv::Mat rgb_mat;
    cv::cvtColor(mat, rgb_mat, cv::COLOR_RGBA2BGR);
    const auto aligned_images =
        modeldeploy::vision::utils::align_face_with_five_points(modeldeploy::ImageData::from_mat(&rgb_mat), detResult);
    modeldeploy::vision::FaceRecognitionResult result;
    if (!face_rec_->predict(aligned_images[0], &result)) {
        qDebug() << "inference Error";
    }
    QVariantMap map;
    map["name"] = "张三";
    map["score"] = "0.75";
    qDebug() << map;
    emit recognizeReady(map);
}
