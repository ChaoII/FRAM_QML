//
// Created by aichao on 2025/10/16.
//

#pragma once
#include "task/Scheduler.h"


class ClearOldAttendInfoTask : QObject {
    Q_OBJECT

public:
    explicit ClearOldAttendInfoTask(QObject* parent = nullptr);

    static void start();

private:
    static bool clearOldAttendInfo();

    static QString getRemoveTime();

    static QString removeTime_;
};
