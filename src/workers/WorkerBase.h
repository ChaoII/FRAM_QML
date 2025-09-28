//
// Created by aichao on 2025/9/28.
//

#pragma once

#include <QThread>
#include <QObject>
#include <QMutex>
#include <atomic>
#include <QDebug>


class WorkerBase : public QObject {
    Q_OBJECT

public:
    explicit WorkerBase(QObject* parent = nullptr);

    virtual void startTask();

    virtual void stopTask();

protected:
    std::atomic<bool> is_running_ = false;
};
