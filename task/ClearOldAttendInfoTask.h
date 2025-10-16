//
// Created by aichao on 2025/10/16.
//

#pragma once
#include "task/ScheduledTask.h"


class ClearOldAttendInfoTask : QObject {
    Q_OBJECT

public:
    ClearOldAttendInfoTask(QObject* parent = nullptr);


    static void start();


private:
    static bool clearOldAttendInfo();
};
