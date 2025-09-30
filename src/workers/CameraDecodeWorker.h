//
// Created by aichao on 2025/9/28.
//

#pragma once

#include <QImage>
#include "src/workers/WorkerBase.h"
#include <opencv2/opencv.hpp>
#include "src/ThreadSafeQueue.h"
#include "src/FrameData.h"


class CameraDecodeWorker : public WorkerBase {
    Q_OBJECT

public:
    explicit CameraDecodeWorker();

    void InitMediaDevice(int index);

    void InitMediaDevice(const QString& fileName);

    void setFrameQueue(ThreadSafeQueue<FrameData>* queue);

public slots:
    void startCameraDecode();

signals:
    void captureError(const QString& error);
    void decodeFinished(QImage image);

private:
    static QImage matToQImage(const cv::Mat& mat);

    void process();

    ThreadSafeQueue<FrameData>* frame_queue_ = nullptr;
    std::unique_ptr<cv::VideoCapture> cap_ = nullptr;
    int frame_sequence_ = 0;
};