#pragma once

#include <QObject>
#include <QThread>
#include <QQmlEngine>
#include "schema/schema.h"

class RegisterInfoWorker;

class RegisterInfoDto : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isQuerying READ isQuerying NOTIFY isQueryingChanged)
    QML_ELEMENT

public:
    explicit RegisterInfoDto(QObject* parent = nullptr);
    ~RegisterInfoDto() override;

    [[nodiscard]] bool isQuerying() const;

    Q_INVOKABLE void queryRegisterInfo();

signals:
    void queryReady(const QVariantList& attendHistory);
    void queryError(const QString& errorInfo);
    void isQueryingChanged();

    void internalQueryRequested();


private:
    QThread* workerThread_;
    RegisterInfoWorker* worker_;
    bool isQuerying_=false;
};

class RegisterInfoWorker : public QObject {
    Q_OBJECT

public:
    explicit RegisterInfoWorker(QObject* parent = nullptr);

signals:
    void queryFinished(const QVariantList& attendHistory);
    void queryError(const QString& errorInfo);

public slots:
    void getRegisterInfo();
};
