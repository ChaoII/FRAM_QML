//
// Created by aichao on 2025/9/25.
//

#include "core/ConfigManager.h"
#include "workers/FaceDetectionWorker.h"
#include "utils.h"
#include <QRandomGenerator>
#include <QThread>
#include <QTimer>

FaceDetectionWorker::FaceDetectionWorker(QObject* parent) {

    modeldeploy::RuntimeOption option;
    option.set_cpu_thread_num(1);
    face_det_ = std::make_unique<modeldeploy::vision::face::Scrfd>(
        "E:/CLionProjects/ModelDeploy/test_data/test_models/face/"
        "scrfd_2.5g_bnkps_shape640x640.onnx",option);
    face_det_->get_postprocessor().set_conf_threshold(ConfigManager::instance()->detThreshold());
}
void FaceDetectionWorker::processFrame(const QImage& image) {
    std::vector<modeldeploy::vision::DetectionLandmarkResult> result;
    auto mat = Utils::QImageToCvMat(image);
    if (mat.empty()) {
        return;
    }
    cv::Mat rgb_mat;
    cv::cvtColor(mat, rgb_mat, cv::COLOR_RGBA2BGR);
    if (!face_det_->predict(modeldeploy::ImageData::from_mat(&rgb_mat),
                            &result)) {
        qDebug() << "inference Error";
    }
    // QThread::msleep(100);
    emit detectReady(result);
}
