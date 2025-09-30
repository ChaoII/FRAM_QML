//
// Created by aichao on 2025/9/28.
//

#pragma once
#include <QQmlApplicationEngine>
#include "src/VideoProcessor.h"

#include "src/providers/CameraImageProvider.h"

class CameraManager : public QObject {
    Q_OBJECT

public:
    explicit CameraManager(QQmlApplicationEngine* engine = nullptr): engine_(engine) {
    }


    Q_INVOKABLE void createCamera(const QString& source,
                                  const VideoProcessor::CameraSourceType type) {
        QString camera_id = QString::number(qHash(source), 16);


        qDebug() << camera_id;
        if (cameras_.contains(camera_id)) {
            return;
        }

        auto video_processor = new VideoProcessor(source, type);
        cameras_[camera_id] = video_processor;
        connect(video_processor, &VideoProcessor::frameReady, this, [this, camera_id]() {
            emit cameraFrameUpdated(camera_id);
        });

        connect(video_processor, &VideoProcessor::detectionReady, this,
                [this, camera_id](const QVariantList& results, qint64 timestamp, int sequence) {
                    emit faceResultUpdate(camera_id, results, timestamp, sequence);
                });


        if (engine_) {
            qDebug() << "addImageProvider: " << camera_id;
            engine_->addImageProvider(camera_id, video_processor->imageProvider());
        }
    }

    Q_INVOKABLE void startCamera(const QString& source) {
        QString camera_id = QString::number(qHash(source), 16);
        if (!cameras_.contains(camera_id)) return;
        cameras_[camera_id]->startProcessing();
    }

    Q_INVOKABLE void stopCamera(const QString& source) {
        QString camera_id = QString::number(qHash(source), 16);
        if (!cameras_.contains(camera_id)) return;
        cameras_[camera_id]->stopProcessing();
    }

signals:
    void cameraFrameUpdated(const QString& camera_id);
    void faceResultUpdate(const QString& camera_id, const QVariantList& results, qint64 timestamp, int sequence);

private:
    QMap<QString, VideoProcessor*> cameras_;
    QQmlApplicationEngine* engine_;
};