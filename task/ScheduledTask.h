//
// Created by aichao on 2025/10/16.
//

#pragma once
#include <QObject>
#include <QTimer>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QDebug>
#include <functional>



class ScheduledTask : public QObject {
    Q_OBJECT
public:
    ScheduledTask(std::function<bool()> taskFunc, qint64 intervalMs, QObject* parent = nullptr);

    ~ScheduledTask() override;

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

