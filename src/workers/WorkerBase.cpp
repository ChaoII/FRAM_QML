//
// Created by aichao on 2025/9/28.
//

#include "src/workers/WorkerBase.h"

WorkerBase::WorkerBase(QObject* parent): QObject(parent) {
}


void WorkerBase::startTask() {
    is_running_ = true;
}

void WorkerBase::stopTask() {
    is_running_ = false;
}
