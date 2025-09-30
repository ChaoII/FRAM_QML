//
// Created by aichao on 2025/9/28.
//

#include <QTimer>
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


void CameraDecodeWorker::setFrameQueue(ThreadSafeQueue<cv::Mat>* queue) {
    frame_queue_ = queue;
}


void CameraDecodeWorker::startCameraDecode() {
    if (is_running_.exchange(true)) return;
    if (!cap_->isOpened()) {
        emit captureError("Failed to open video source");
        is_running_ = false;
        return;
    }
    // 启动处理循环
    QTimer::singleShot(0, this, &CameraDecodeWorker::process);
}


void CameraDecodeWorker::process() {
    if (!is_running_) return;
    // qDebug() << "process" << is_running_;
    cv::Mat frame;
    if (!cap_->read(frame)) {
        emit captureError("Failed to read frame");
        is_running_ = false;
        return;
    }

    if (!frame.empty()) {
        // 发送信号给QML显示
        emit decodeFinished(matToQImage(frame));
        // 如果有推理队列，也推送到推理线程
        if (frame_queue_) {
            frame_queue_->push(std::move(frame));
        }
    }
    // 继续下一帧
    if (is_running_) {
        QTimer::singleShot(1, this, &CameraDecodeWorker::process);
    }
}


QImage CameraDecodeWorker::matToQImage(const cv::Mat& mat) {
    if (mat.empty()) return {};
    cv::Mat rgb;
    cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
}
