
#include <QDateTime>
#include "registerinfodto.h"
#include "core/dbHelper.h"

RegisterInfoDto::RegisterInfoDto(QObject* parent)
    : QObject{parent} {
    workerThread_ = new QThread(this);
    worker_ = new RegisterInfoWorker();
    worker_->moveToThread(workerThread_);
    connect(workerThread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(this, &RegisterInfoDto::internalQueryRequested, worker_, &RegisterInfoWorker::getRegisterInfo);
    connect(worker_, &RegisterInfoWorker::queryFinished, [this](const QVariantList &registerInfos){
        emit queryReady( registerInfos);
        isQuerying_ = false;
    } );
    connect(worker_, &RegisterInfoWorker::queryError, [this](const QString &erroInfo){
        emit queryError( erroInfo);
        isQuerying_ = false;
    } );

    workerThread_->start();
}


RegisterInfoDto::~RegisterInfoDto() {
    workerThread_->quit();
    workerThread_->wait();
}

bool RegisterInfoDto::isQuerying() const {
    return isQuerying_;
}

void RegisterInfoDto::queryRegisterInfo() {
    if (isQuerying_) return;
    isQuerying_ = true;
    emit internalQueryRequested();
    emit isQueryingChanged();
}

RegisterInfoWorker::RegisterInfoWorker(QObject* parent)
    : QObject{parent} {
}

void RegisterInfoWorker::getRegisterInfo() {

    try {
        const auto registerInfos = DBHelper::getStorage().get_all<Staff>();
        QVariantList list;
        for (const auto& s:registerInfos) {
            QVariantMap map;
            map["name"] = QString::fromStdString(s.name);
            map["staffNo"] = QString::fromStdString(s.staffNo);
            map["picUrl"] = QString::fromStdString(s.picUrl);
            map["registerTime"] = QString::fromStdString(s.registerTime);
            list.append(map);
        }
        emit queryFinished(list);
    }
    catch (const std::exception& e) {
        qDebug() << "insert staff failed: " << e.what();
        emit queryError(QString::fromStdString(e.what()));
    }
}
