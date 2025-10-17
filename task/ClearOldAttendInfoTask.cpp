//
// Created by aichao on 2025/10/16.
//

#include "task/ClearOldAttendInfoTask.h"
#include "core/dbHelper.h"
#include "core/ConfigManager.h"


QString ClearOldAttendInfoTask::removeTime_="";

ClearOldAttendInfoTask::ClearOldAttendInfoTask(QObject* parent): QObject(parent) {

    removeTime_ = getRemoveTime();
}

QString ClearOldAttendInfoTask::getRemoveTime() {
    const int internal = ConfigManager::instance()->clearOldInterval();
    QString removeTime = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz");

    switch (ConfigManager::instance()->clearOldUnit()) {
    case 0:
        removeTime = QDateTime::currentDateTime().addDays(-internal).toString("yyyy-MM-ddTHH:mm:ss.zzz");
        break;
    case 1:
        removeTime = QDateTime::currentDateTime().addMonths(-internal).toString("yyyy-MM-ddTHH:mm:ss.zzz");
        break;
    case 2:
        removeTime = QDateTime::currentDateTime().addYears(-internal).toString("yyyy-MM-ddTHH:mm:ss.zzz");
        break;
    default: ;
    }
    return removeTime;
}


bool ClearOldAttendInfoTask::clearOldAttendInfo() {
    try {
        const QDateTime now = QDateTime::currentDateTime();
        const QString removeTime = removeTime_;
        auto& storage = DBHelper::getInstance().getStorage();
        const auto oldRecords = storage.get_all<AttendInfo>(
            where(c(&AttendInfo::attendTime) <= removeTime.toStdString())
            );
        for (const auto& info : oldRecords) {
            QFile file(QString::fromStdString(info.picUrl));
            if (file.exists() && !file.remove()) {
                qWarning() << "[ClearTask] 删除文件失败:" << file.fileName();
            }
            // 删除数据库记录
            storage.remove<AttendInfo>(info.id);
            qDebug() << "[ClearTask] 删除数据库记录:" << info.id;
        }
        return true;
    }
    catch (const std::exception& e) {
        qWarning() << "[ClearTask] 任务异常:" << e.what();
        return false;
    }
}

void ClearOldAttendInfoTask::start() {
    const auto clearOldAttendTask = new Scheduler(
        clearOldAttendInfo, 24 * 60 * 60); // 24小时执行一次
    clearOldAttendTask->start();
}
