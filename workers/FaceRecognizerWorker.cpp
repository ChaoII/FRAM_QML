//
// Created by aichao on 2025/9/25.
//

#include "workers/FaceRecognizerWorker.h"

#include <QDateTime>
#include <QRandomGenerator>
#include <QTimer>
#include <QThread>
#include "utils.h"
#include "core/dbHelper.h"
#include "core/VectorSearch.h"

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
    // 向量检索
    const auto searchResult = VectorSearch::getInstance().search(result.embedding, 1);
    const bool isUnknown = searchResult.second[0] < 0.62;
    if (isUnknown) {
        qDebug() << "no staff found";
        map["isUnknown"] = isUnknown;
        map["staffNo"] = "";
        map["name"] = "";
        map["score"] = 0.0;
        map["picUrl"] = "";
        map["attendTime"] = "";
        emit recognizeReady(map);
        return;
    }
    const auto staffs = DBHelper::getInstance().queryStaffByIndexID(searchResult.first[0]);
    const QString attendTime = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz");
    map["unknown"] = isUnknown;
    map["staffNo"] = QString::fromStdString(staffs[0].staffNo);
    map["name"] = QString::fromStdString(staffs[0].name);
    map["score"] = searchResult.second[0];
    map["picUrl"] = QString::fromStdString(staffs[0].picUrl);
    map["attendTime"] = attendTime;
    qDebug() << map;
    emit recognizeReady(map);
}
