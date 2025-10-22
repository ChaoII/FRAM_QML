
#include <QDateTime>
#include <VectorSearch.h>
#include "qmlimageutils.h"
#include "registerinfodto.h"
#include "core/dbHelper.h"

RegisterInfoDto::RegisterInfoDto(QObject* parent)
    : QObject{parent} {
    workerThread_ = new QThread(this);
    worker_ = new RegisterInfoWorker();
    worker_->moveToThread(workerThread_);
    connect(workerThread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(this, &RegisterInfoDto::internalQueryRequested, worker_, &RegisterInfoWorker::getRegisterInfo);
    connect(this, &RegisterInfoDto::internalQueryRequestedByName, worker_, &RegisterInfoWorker::getRegisterInfoByName);

    connect(this, &RegisterInfoDto::internalDeleteRequested, worker_, &RegisterInfoWorker::deleteRegisterInfos);
    connect(worker_,&RegisterInfoWorker::deleteFinished,[this](){
        emit deleteFinished();
        setBusy(false);
    });
    connect(worker_,&RegisterInfoWorker::deleteError,[this](){
        emit deleteError();
        setBusy(false);
    });
    connect(worker_, &RegisterInfoWorker::queryFinished, [this](const QVariantList &registerInfos){
        emit queryReady(registerInfos);
        setBusy(false);
    });
    connect(worker_, &RegisterInfoWorker::queryError, [this](const QString &erroInfo){
        emit queryError( erroInfo);
        setBusy(false);
    });
    workerThread_->start();
}


RegisterInfoDto::~RegisterInfoDto() {
    workerThread_->quit();
    workerThread_->wait();
}

bool RegisterInfoDto::isBusy() const {
    return isBusy_;
}
void RegisterInfoDto::setBusy(bool isBusy){
    if(isBusy_!=isBusy){
        isBusy_ = isBusy;
        emit isBusyChanged();
    }
}

void RegisterInfoDto::queryRegisterInfo() {
    if (isBusy_) return;
    setBusy(true);
    emit internalQueryRequested();
}

void RegisterInfoDto::queryRegisterInfoByName(const QString& name){
    if (isBusy_) return;
    setBusy(true);
    emit internalQueryRequestedByName(name);
}


void RegisterInfoDto::deleteRegisterInfos(const std::vector<int64_t>& indexIDs){
    if (isBusy_) return;
    setBusy(true);
    emit internalDeleteRequested(indexIDs);
}

RegisterInfoWorker::RegisterInfoWorker(QObject* parent)
    : QObject{parent} {
}

void RegisterInfoWorker::deleteRegisterInfos(const std::vector<int64_t>& indexIDs){
    bool success =  DBHelper::getStorage().transaction([&] () mutable {
        try {
            for(auto& indexID:indexIDs){
                // 查询当前记录的图片路径
                auto staff = DBHelper::getStorage().get_all<Staff>(where(c(&Staff::indexId)==indexID));

                if (staff.empty()) {
                    qDebug() << "No staff record found for indexID:" << indexID;
                    continue;  // 跳过无效记录
                }

                QString picUrl = QString::fromStdString(staff.front().picUrl);
                // 删除数据库记录
                DBHelper::getStorage().remove_all<Staff>(where(c(&Staff::indexId)==indexID));
                // 删除本地图片
                QmlImageUtils::removeFile(picUrl);
            }
            return true;
        } catch (const std::exception& e) {
            qDebug()<< "delete register information failed: "<<e.what();
            return false;
        }
    });
    // 删除向量数据库
    if(success) {
        VectorSearch::getInstance().removeVectors(indexIDs);
        VectorSearch::getInstance().save();
        emit deleteFinished();
    }else{
        emit deleteError();
    }
}


void RegisterInfoWorker::getRegisterInfo() {
    try {
        const auto registerInfos = DBHelper::getStorage().get_all<Staff>();
        QVariantList list;
        for (const auto& s:registerInfos) {
            QVariantMap map;
            map["key"] = QString::number(s.indexId);
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

void RegisterInfoWorker::getRegisterInfoByName(const QString& name){
    qDebug()<<"name: "<<name;
    try {
        const auto registerInfos = DBHelper::getStorage().get_all<Staff>(
            where(c(&Staff::name)==name.toStdString())
            );
        QVariantList list;
        for (const auto& s:registerInfos) {
            QVariantMap map;
            map["key"] = QString::number(s.indexId);
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

