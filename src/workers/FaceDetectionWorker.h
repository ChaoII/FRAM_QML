#pragma once
#include <QPainter>
#include "src/workers/WorkerBase.h"
#include "modeldeploy/vision.h"
#include "opencv2/opencv.hpp"
#include "src/ThreadSafeQueue.h"
#include <QVariantMap>


struct DetectionResult {
    int x, y, width, height;
    float score;
    int label_id;
};

struct DetectionResults {
    std::vector<DetectionResult> results;
};

Q_DECLARE_METATYPE(DetectionResults)

class FaceDetectionWorker : public WorkerBase {
    Q_OBJECT

public:
    explicit FaceDetectionWorker(QObject* parent = nullptr);

    void setFrameQueue(ThreadSafeQueue<cv::Mat>* queue) {
        frame_queue_ = queue;
    }

public slots:
    void startFaceDetection();

signals:
    void inferenceError(const QString&);
    void detectionFinished(const QVariantList& results);

private:
    void process();
    static QVariantList convertToVariantList(const std::vector<modeldeploy::vision::DetectionLandmarkResult>& dets);
    ThreadSafeQueue<cv::Mat>* frame_queue_ = nullptr;
    std::unique_ptr<modeldeploy::vision::face::Scrfd> face_model_ = nullptr;
};
