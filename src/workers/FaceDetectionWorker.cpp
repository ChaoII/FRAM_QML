//
// Created by aichao on 2025/9/25.
//

#include "FaceDetectionWorker.h"
#include <QRandomGenerator>
#include <QTimer>


FaceDetectionWorker::FaceDetectionWorker(QObject* parent)
    : WorkerBase(parent) {
    face_model_ = std::make_unique<modeldeploy::vision::face::Scrfd>(
        "E:/CLionProjects/ModelDeploy/test_data/test_models/face/scrfd_2.5g_bnkps_shape640x640.onnx");
}

void FaceDetectionWorker::startFaceDetection() {
    if (is_running_.exchange(true)) return;
    QTimer::singleShot(0, this, &FaceDetectionWorker::process);
}

QVariantList FaceDetectionWorker::convertToVariantList(
    const std::vector<modeldeploy::vision::DetectionLandmarkResult>& dets) {
    QVariantList list;
    for (const auto& r : dets) {
        if (r.score < 0.5f) continue;
        QVariantMap map;
        map["x"] = r.box.x;
        map["y"] = r.box.y;
        map["width"] = r.box.width;
        map["height"] = r.box.height;
        map["score"] = r.score;
        map["label_id"] = r.label_id;
        list.append(map);
    }
    return list;
}

void FaceDetectionWorker::process() {
    if (!is_running_) return;
    FrameData frame_data;
    // 使用阻塞等待，确保不丢失帧
    if (frame_queue_ && frame_queue_->wait_and_pop(frame_data)) {
        try {
            // 预处理
            std::vector<modeldeploy::vision::DetectionLandmarkResult> result;
            auto frame_ = frame_data.frame.clone();
            if (!face_model_->predict(modeldeploy::ImageData::from_mat(&frame_), &result)) {
                qDebug() << "inference Error";
                emit inferenceError("inference Error");
            }
            auto results = convertToVariantList(result);
            emit detectionFinished(results, frame_data.timestamp, frame_data.sequence);
        }
        catch (const std::exception& e) {
            emit inferenceError(QString::fromStdString(e.what()));
        }
    }

    // 继续处理
    if (is_running_) {
        // 使用更短的延迟以提高响应速度
        QTimer::singleShot(50, this, &FaceDetectionWorker::process);
    }
}