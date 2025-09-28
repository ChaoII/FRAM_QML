//
// Created by aichao on 2025/9/28.
//

#include "FaceDetectionResultProvider.h"


FaceDetectionResultProvider::FaceDetectionResultProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {
}

QImage FaceDetectionResultProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize) {
    Q_UNUSED(id)
    QMutexLocker locker(&m_mutex);
    if (size) *size = m_image.size();
    if (requestedSize.isValid()) {
        qDebug() << "image is null: " << m_image.isNull();
        return m_image.scaled(requestedSize);
    }
    return m_image;
}

void FaceDetectionResultProvider::setImage(const QImage& image) {
    QMutexLocker locker(&m_mutex);
    m_image = image;
}
