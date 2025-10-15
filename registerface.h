#pragma once

#include <QObject>
#include <modeldeploy/vision.h>
#include <QDebug>
#include <QThread>

class RegisterFaceWorker;

class RegisterFace:public QObject
{
    Q_OBJECT
public:
    ~RegisterFace();
    RegisterFace(QObject* parent=nullptr);
    Q_INVOKABLE void processImages(const QString& name,
                                   const QString& staffNo,
                                   const QStringList& imagePaths);

signals:
    void registerInfoReady(const QString& name,
                           const QString& staffNo,
                           const QStringList& imagePaths);

    void registerReady();

private:
    QThread* registerThread_;
    RegisterFaceWorker* registerWorker_;
};




class RegisterFaceWorker:public QObject{

    Q_OBJECT



public:
    RegisterFaceWorker(QObject* parent=nullptr):QObject(parent){}

signals:
    void registerFinished();

public slots:

    void RegisterFace(const QString& name,const QString& staffNo,const QStringList& imagePaths){
        // 读取图片
        std::vector<std::vector<float>> features;
        qDebug()<<"-------3---------";
        for (auto& imagePath:imagePaths){
            auto image = modeldeploy::ImageData::imread(imagePath.toStdString());
            auto model = modeldeploy::vision
                ::face::FaceRecognizerPipeline(
                    "E:/CLionProjects/ModelDeploy/test_data/test_models/face/scrfd_2.5g_bnkps_shape640x640.onnx",
                    "E:/CLionProjects/ModelDeploy/test_data/test_models/face/face_recognizer.onnx");
            std::vector<modeldeploy::vision::FaceRecognitionResult> results;
            if(!model.predict(image,&results)){
                qDebug()<<"register failed, file path is:"<< imagePath <<"register will fallback!";
                return;
            }
            auto feature = results[0].embedding;
            features.push_back(feature);
        }
        emit registerFinished();
        // 保存到向量数据库
        // 发送注册完成信号
    }
};

