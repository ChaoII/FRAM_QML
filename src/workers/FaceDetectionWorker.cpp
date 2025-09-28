//
// Created by aichao on 2025/9/25.
//

#include "FaceDetectionWorker.h"
#include <QRandomGenerator>


void FaceDetectionWorker::startFaceDetection() {
    is_running_ = true;
    while (is_running_) {
        QThread::msleep(33); // 30 FPS
        QImage img(":/images/image_0004.jpg"); // 示例图片
        if (!img.isNull()) {
            QImage out = faceDetection(img);
            emit detectionFinished(out);
        }
    }
}


QImage FaceDetectionWorker::faceDetection(const QImage& image) {
    QImage result = image.copy();
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    is_running_ = false;
    int faceCount = QRandomGenerator::global()->bounded(1, 4);
    for (int i = 0; i < faceCount; ++i) {
        int x = QRandomGenerator::global()->bounded(0, image.width() - 100);
        int y = QRandomGenerator::global()->bounded(0, image.height() - 100);
        int w = QRandomGenerator::global()->bounded(80, 150);
        int h = QRandomGenerator::global()->bounded(80, 150);
        painter.setPen(QPen(Qt::red, 3));
        painter.drawRect(x, y, w, h);
        painter.setPen(Qt::white);
        painter.drawText(x, y - 5, QString("Face %1").arg(i + 1));
    }
    return result;
}
