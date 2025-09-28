//
// Created by aichao on 2025/9/28.
//

#pragma once
#include <QQmlApplicationEngine>
#include "src/CameraDecoder.h"
#include "src/providers/CameraImageProvider.h"

class CameraManager : public QObject {
    Q_OBJECT

public:
    explicit CameraManager(QQmlApplicationEngine* engine = nullptr): engine_(engine) {
    }


    Q_INVOKABLE void createCamera(const QString& source,
                                  const CameraDecoder::CameraSourceType type) {

        QString camera_id = QString::number(qHash(source), 16);


        qDebug() << camera_id;
        if (cameras_.contains(camera_id)) {
            return;
        }

        auto camera_decoder = new CameraDecoder(source, type);
        cameras_[camera_id] = camera_decoder;
        connect(camera_decoder, &CameraDecoder::imageReady, this, [this, camera_id]() {
            emit cameraFrameUpdated(camera_id);
        });
        if (engine_) {
            qDebug() << "addImageProvider: " << camera_id;
            engine_->addImageProvider(camera_id, camera_decoder->imageProvider());
        }
    }

    Q_INVOKABLE void startCamera(const QString& source) {
        QString camera_id = QString::number(qHash(source), 16);
        if (!cameras_.contains(camera_id)) return;
        cameras_[camera_id]->startDecode();
    }

    Q_INVOKABLE void stopCamera(const QString& source) {
        QString camera_id = QString::number(qHash(source), 16);
        if (!cameras_.contains(camera_id)) return;
        cameras_[camera_id]->stopDecode();
    }

signals:
    void cameraFrameUpdated(const QString& camera_id);

private:
    QMap<QString, CameraDecoder*> cameras_;
    QQmlApplicationEngine* engine_;
};
