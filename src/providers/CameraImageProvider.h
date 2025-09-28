#pragma once
#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class CameraImageProvider : public QQuickImageProvider {
public:
    explicit CameraImageProvider()
        : QQuickImageProvider(QQuickImageProvider::Image) {
        m_image = QImage();
    }

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override {
        QMutexLocker locker(&m_mutex);
        if (size) *size = m_image.size();
        return m_image;
    }

    void setImage(const QImage& image) {
        QMutexLocker locker(&m_mutex);
        m_image = image;
    }

private:
    QImage m_image;
    QMutex m_mutex;
};
