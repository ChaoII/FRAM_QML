//
// Created by aichao on 2025/9/28.
//

#pragma once

#include <QMutex>
#include <QQuickImageProvider>

class FaceDetectionResultProvider : public QQuickImageProvider {
public:
    explicit FaceDetectionResultProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override ;

    void setImage(const QImage &image) ;

private:
    QImage m_image;
    QMutex m_mutex;
};
