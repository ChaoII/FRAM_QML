//
// Created by aichao on 2025/9/28.
//



#pragma once
#include <iostream>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QVector>
#include <QVariantMap>

class LocalCamera : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList cameras READ cameras NOTIFY camerasChanged)

public:
    struct LocalCameraInfo {
        QString description;
        QString deviceID;
        int index;
    };

    explicit LocalCamera(QObject* parent = nullptr) : QObject(parent) {
        const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        for (int i = 0; i < cameras.size(); i++) {
            local_camera_infos_.push_back({cameras[i].description(), QString::fromUtf8(cameras[i].id()), i});

        }
    }

    [[nodiscard]] QVariantList cameras() const {
        QVariantList list;
        for (auto& c : local_camera_infos_) {
            QVariantMap map;
            map["description"] = c.description;
            map["deviceID"] = c.deviceID;
            map["index"] = c.index;
            list.append(map);
        }
        return list;
    }

    Q_INVOKABLE int getCameraIndexByID(const QString& cameraID) {
        for (auto& local_camera_info : local_camera_infos_) {
            if (local_camera_info.deviceID == cameraID) {
                return local_camera_info.index;
            }
        }
        return -1;
    }

signals:
    void camerasChanged();

private:
    QVector<LocalCameraInfo> local_camera_infos_;
};
