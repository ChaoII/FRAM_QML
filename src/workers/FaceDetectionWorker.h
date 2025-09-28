#pragma once
#include <QPainter>
#include "src/workers/WorkerBase.h"

class FaceDetectionWorker : public WorkerBase {
    Q_OBJECT

public slots:
    void startFaceDetection();

signals:
    void detectionFinished(const QImage& image);

private:
    QImage faceDetection(const QImage& image);
};
