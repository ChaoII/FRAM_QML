//
// Created by aichao on 2025/10/16.
//

#include "task/ClearOldAttendInfoTask.h"
#include "core/dbHelper.h"

ClearOldAttendInfoTask::ClearOldAttendInfoTask(QObject* parent): QObject(parent) {
}

bool ClearOldAttendInfoTask::clearOldAttendInfo() {
    try {
        const QDateTime now = QDateTime::currentDateTime();
        const QString thresholdStr = now.addSecs(-1*60*60).toString("yyyy-MM-ddTHH:mm:ss.zzz");
        auto& storage = DBHelper::getInstance().getStorage();
        const auto oldRecords = storage.get_all<AttendInfo>(
            where(c(&AttendInfo::attendTime) <= thresholdStr.toStdString())
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
    const auto clearOldAttendTask = new ScheduledTask(
        clearOldAttendInfo, 10000); // 24小时执行一次
    clearOldAttendTask->start();
}
