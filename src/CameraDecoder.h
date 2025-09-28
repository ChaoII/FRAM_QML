//
// Created by aichao on 2025/9/28.
//

#pragma once

#include "src/providers/CameraImageProvider.h"
#include "src/workers/CameraDecodeWorker.h"


class CameraDecoder : public QObject {
    Q_OBJECT

public :
    enum class CameraSourceType {
        Local,
        RTSP,
        File
    };

    Q_ENUM(CameraSourceType)

    explicit CameraDecoder(const QString& source, CameraSourceType type, QObject* parent = nullptr);

    void handleImage(const QImage&);

    [[nodiscard]] CameraImageProvider* imageProvider() const { return provider_; }


    Q_INVOKABLE void startDecode();

    Q_INVOKABLE void stopDecode();

    ~CameraDecoder() override;
signals:
    void imageReady();

private:
    CameraDecodeWorker* worker_;
    QThread* workerThread_;
    CameraImageProvider* provider_;
};
