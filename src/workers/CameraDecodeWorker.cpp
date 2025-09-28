//
// Created by aichao on 2025/9/28.
//

#include "CameraDecodeWorker.h"


CameraDecodeWorker::CameraDecodeWorker() {
}


void CameraDecodeWorker::InitMediaDevice(int index) {
    if (cap_) {
        cap_->release();
    }
    cap_ = std::make_unique<cv::VideoCapture>(index);
}


void CameraDecodeWorker::InitMediaDevice(const QString& fileName) {
    if (cap_) {
        cap_->release();
    }
    cap_ = std::make_unique<cv::VideoCapture>(fileName.toStdString());
}


void CameraDecodeWorker::startCameraDecode() {
    is_running_ = true;
    cv::Mat frame;
    while (is_running_) {
        cap_->read(frame);
        if (frame.empty()) continue;

        QImage image = matToQImage(frame);
        emit decodeFinished(image);

        QThread::msleep(30); // 控制帧率
    }
}

QImage CameraDecodeWorker::matToQImage(const cv::Mat& mat) {
    if (mat.empty()) return {};
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}
