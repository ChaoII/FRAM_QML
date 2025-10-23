//
// Created by aichao on 2025/10/16.
// 调度器，用于定时执行任务
//

#pragma once
#include <QtConcurrent>
#include <functional>


class Scheduler : public QObject {
    Q_OBJECT

public:
    Scheduler(std::function<bool()> taskFunc, qint64 intervalMs, QObject* parent = nullptr);

    ~Scheduler() override;

    void start();
    void stop();

private slots:
    void runTask();

private:
    QTimer* timer_;
    QFutureWatcher<bool>* watcher_;
    std::function<bool()> taskFunc_;
    qint64 intervalMs_;
    bool running_ = false;
};
