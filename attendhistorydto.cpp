
#include <QDateTime>
#include "attendhistorydto.h"
#include "core/dbHelper.h"

AttendHistoryDto::AttendHistoryDto(QObject* parent)
    : QObject{parent} {
    workerThread_ = new QThread(this);
    worker_ = new AttendHistoryWorker();
    worker_->moveToThread(workerThread_);
    connect(workerThread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(this, &AttendHistoryDto::internalQueryRequested, worker_, &AttendHistoryWorker::getAttendHistory);
    connect(worker_, &AttendHistoryWorker::queryFinished, [this](const QVariantList &attendInfos){
        emit queryReady( attendInfos);
        isQuerying_ = false;
    } );
    connect(worker_, &AttendHistoryWorker::queryError, [this](const QString &erroInfo){
        emit queryError( erroInfo);
        isQuerying_ = false;
    } );

    workerThread_->start();
}


AttendHistoryDto::~AttendHistoryDto() {
    workerThread_->quit();
    workerThread_->wait();
}

bool AttendHistoryDto::isQuerying() const {
    return isQuerying_;
}

void AttendHistoryDto::queryAttendHistory(const QString& startTime, const QString& endTime, const QString& name) {
    if (isQuerying_) return;
    isQuerying_ = true;
    emit internalQueryRequested(startTime, endTime, name);
    emit isQueryingChanged();
}

AttendHistoryWorker::AttendHistoryWorker(QObject* parent)
    : QObject{parent} {
}

void AttendHistoryWorker::getAttendHistory(const QString& startTime, const QString& endTime, const QString& name) {

    QString startTime_=startTime;
    QString endTime_=endTime;
    if(startTime.isEmpty()){
        startTime_ = QDateTime::currentDateTime().date().startOfDay().toString("yyyy-MM-ddTHH:mm:ss.zzz");
    }
    if(endTime.isEmpty()){
        endTime_ = QDateTime::currentDateTime().date().startOfDay().addDays(1).toString("yyyy-MM-ddTHH:mm:ss.zzz");
    }

    try {
        const auto attendHistory = DBHelper::getStorage().get_all<AttendInfo>(
            where(
                c(&AttendInfo::attendTime) >= startTime_.toStdString()
                and c(&AttendInfo::attendTime) <= endTime_.toStdString()
                and (name.isEmpty()||not(c(&AttendInfo::name) == name.toStdString()))
                )
            );
        QVariantList list;
        for (const auto& s:attendHistory) {
            QVariantMap map;
            map["name"] = QString::fromStdString(s.name);
            map["staffNo"] = QString::fromStdString(s.staffNo);
            map["picUrl"] = QString::fromStdString(s.picUrl);
            map["attendTime"] = QString::fromStdString(s.attendTime);
            list.append(map);
        }
        emit queryFinished(list);
    }
    catch (const std::exception& e) {
        qDebug() << "insert staff failed: " << e.what();
        emit queryError(QString::fromStdString(e.what()));
    }
}
