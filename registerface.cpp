#include "registerface.h"
#include <QDebug>
#include <QThread>

RegisterFace::RegisterFace(QObject* parent): QObject(parent) {
    registerThread_ = new QThread(this);
    registerWorker_ = new RegisterFaceWorker();
    registerWorker_->moveToThread(registerThread_);
    connect(this, &RegisterFace::registerInfoReady,
            registerWorker_, &RegisterFaceWorker::RegisterFace);
    connect(registerWorker_, &RegisterFaceWorker::registerFinished,
            this, &RegisterFace::registerReady);
    connect(registerWorker_, &RegisterFaceWorker::registerFailed,
            this, &RegisterFace::registerError);
    registerThread_->start();
}



RegisterFace::~RegisterFace() {
    if (registerThread_) {
        registerThread_->quit();
        registerThread_->wait();
        delete registerWorker_;
    }
}

void RegisterFace::processImages(const QString& name, const QString& staffNo,
                                 const QStringList& imagePaths) {
    emit registerInfoReady(name, staffNo, imagePaths);
}
