#pragma once

#include <QObject>
#include <QThread>
#include <QQmlEngine>
#include "schema/schema.h"

class AttendHistoryWorker;

class AttendHistoryDto : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isQuerying READ isQuerying NOTIFY isQueryingChanged)
    QML_ELEMENT

public:
    explicit AttendHistoryDto(QObject* parent = nullptr);
    ~AttendHistoryDto() override;

    [[nodiscard]] bool isQuerying() const;

    Q_INVOKABLE void queryAttendHistory(const QString& startTime, const QString& endTime, const QString& name);

signals:
    void queryReady(const QVariantList& attendHistory);
    void queryError(const QString& errorInfo);
    void isQueryingChanged();

    void internalQueryRequested(const QString& startTime, const QString& endTime, const QString& name);


private:
    QThread* workerThread_;
    AttendHistoryWorker* worker_;
    bool isQuerying_=false;
};

class AttendHistoryWorker : public QObject {
    Q_OBJECT

public:
    explicit AttendHistoryWorker(QObject* parent = nullptr);

signals:
    void queryFinished(const QVariantList& attendHistory);
    void queryError(const QString& errorInfo);

public slots:
    void getAttendHistory(const QString& startTime, const QString& endTime, const QString& name);
};
