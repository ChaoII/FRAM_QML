//
// Created by aichao on 2025/9/28.
//

#pragma once

#include <QImage>
#include "src/workers/WorkerBase.h"
#include <opencv2/opencv.hpp>


class CameraDecodeWorker : public WorkerBase {
    Q_OBJECT

public:
    explicit CameraDecodeWorker();

    void InitMediaDevice(int index);


    void InitMediaDevice(const QString& fileName);

public slots:
    void startCameraDecode();

signals:
    void decodeFinished(QImage image);

private:
    static QImage matToQImage(const cv::Mat& mat);

private:
    std::unique_ptr<cv::VideoCapture> cap_ = nullptr;
};
