//
// Created by aichao on 2025/10/16.
//

#include "Scheduler.h"
#include <QtConcurrent>
#include <QDebug>

Scheduler::Scheduler(std::function<bool()> taskFunc, qint64 intervalMs, QObject* parent)
    : QObject(parent), taskFunc_(std::move(taskFunc)), intervalMs_(intervalMs) {
    timer_ = new QTimer();
    timer_->setInterval(intervalMs_);
    connect(timer_, &QTimer::timeout, this, &Scheduler::runTask);

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

Scheduler::~Scheduler() {
    stop();
    if (watcher_->isRunning()) {
        watcher_->future().cancel();
        watcher_->waitForFinished();
    }
}

void Scheduler::start() {
    runTask(); // 第一次立即执行
    timer_->start();
}

void Scheduler::stop() {
    timer_->stop();
}

void Scheduler::runTask() {
    if (running_) {
        qWarning() << "[ScheduledTask] 上一次任务还未完成，跳过本次执行";
        return;
    }
    running_ = true;
    watcher_->setFuture(QtConcurrent::run(taskFunc_));
}
