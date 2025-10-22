#pragma once

#include <QDateTime>
#include <QObject>
#include "core/snowflake.hpp"
#include <modeldeploy/vision.h>
#include <QDebug>
#include <QThread>

#include "core/dbHelper.h"
#include "core/VectorSearch.h"

#include "qmlimageutils.h"

class RegisterFaceWorker;

class RegisterFace : public QObject {
    Q_OBJECT

public:
    explicit RegisterFace(QObject* parent = nullptr);
    ~RegisterFace() override;

    Q_INVOKABLE void processImages(const QString& name,
                                   const QString& staffNo,
                                   const QStringList& imagePaths);


signals:
    void registerInfoReady(const QString& name,
                           const QString& staffNo,
                           const QStringList& imagePaths);

    void registerFailed(const QString& errorInfo);

    void registerFinished();

private:
    QThread* registerThread_;
    RegisterFaceWorker* registerWorker_;
};


class RegisterFaceWorker : public QObject {
    Q_OBJECT

public:
    explicit RegisterFaceWorker(QObject* parent = nullptr): QObject(parent) {
    }

signals:
    void registerFinished();

    void registerFailed(const QString& errorInfo);

public slots:
    void RegisterFace(const QString& name, const QString& staffNo, const QStringList& imagePaths) {
        // 读取图片
        std::vector<std::vector<float>> features;
        std::vector<int64_t> indexes;
        auto model = modeldeploy::vision
            ::face::FaceRecognizerPipeline(
                "E:/CLionProjects/ModelDeploy/test_data/test_models/face/scrfd_2.5g_bnkps_shape640x640.onnx",
                "E:/CLionProjects/ModelDeploy/test_data/test_models/face/face_recognizer.onnx");
        for (auto& imagePath : imagePaths) {
            auto newImagePath= QmlImageUtils::getTruePicUrlFromTemp(imagePath,staffNo);
            if(QFile::rename(imagePath,newImagePath)){
                auto image = modeldeploy::ImageData::imread(newImagePath.toStdString());
                std::vector<modeldeploy::vision::FaceRecognitionResult> results;
                if (!model.predict(image, &results)) {
                    qDebug() << "register failed, file path is:" << newImagePath << "register will fallback!";
                    emit registerFailed(QString("register failed, file path is: ") + newImagePath + "register will fallback!");
                    return;
                }
                auto indexId = SnowflakeID::generate();
                auto feature = results[0].embedding;
                features.emplace_back(feature);
                indexes.emplace_back(indexId);
                Staff staff;
                staff.indexId = indexId;
                staff.name = name.toStdString();
                staff.staffNo = staffNo.toStdString();
                staff.picUrl = newImagePath.toStdString();
                staff.registerTime = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz").toStdString();
                try {
                    DBHelper::getStorage().insert(staff);
                    // 保存到向量数据库
                    VectorSearch::getInstance().addVectors(indexes, features);
                    VectorSearch::getInstance().save();
                }
                catch (const std::exception& e) {
                    qDebug() << "Insert staff failed, "
                                "indexId is: " << indexId << ","
                                "name is: " << name << ","
                                "file path is: " << newImagePath << ","
                                "error message: " << e.what() << ", "
                                "register will fallback!";
                    emit registerFailed(QString("register fail: ") + e.what());
                    return;
                }
            }
        }
        // 发送注册完成信号
        emit registerFinished();
    }
};
