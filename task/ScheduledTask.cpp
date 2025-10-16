//
// Created by aichao on 2025/10/16.
//

// ScheduledTask.cpp
#include "ScheduledTask.h"
#include <QtConcurrent>
#include <QDebug>

ScheduledTask::ScheduledTask(std::function<bool()> taskFunc, qint64 intervalMs, QObject* parent)
    : QObject(parent), taskFunc_(std::move(taskFunc)), intervalMs_(intervalMs) {
    timer_ = new QTimer();
    timer_->setInterval(intervalMs_);
    connect(timer_, &QTimer::timeout, this, &ScheduledTask::runTask);

    watcher_ = new QFutureWatcher<bool>(this);
    connect(watcher_, &QFutureWatcher<bool>::finished, this, [this]() {
        bool success = watcher_->result();
        running_ = false;
        if (!success) {
            qWarning() << "[ScheduledTask] 任务执行失败，将在下一次间隔重试";
        }
        else {
            qDebug() << "[ScheduledTask] 任务执行完成";
        }
    });
}

ScheduledTask::~ScheduledTask() {
    stop();
    if (watcher_->isRunning()) {
        watcher_->future().cancel();
        watcher_->waitForFinished();
    }
}

void ScheduledTask::start() {
    runTask(); // 第一次立即执行
    timer_->start();
}

void ScheduledTask::stop() {
    timer_->stop();
}

void ScheduledTask::runTask() {
    if (running_) {
        qWarning() << "[ScheduledTask] 上一次任务还未完成，跳过本次执行";
        return;
    }
    running_ = true;
    watcher_->setFuture(QtConcurrent::run(taskFunc_));
}
