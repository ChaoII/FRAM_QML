#pragma once

#include <QObject>
#include <QThread>
#include <QQmlEngine>
#include "schema/schema.h"

class RegisterInfoWorker;

class RegisterInfoDto : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)
    QML_ELEMENT

public:
    explicit RegisterInfoDto(QObject* parent = nullptr);
    ~RegisterInfoDto() override;

    [[nodiscard]] bool isBusy() const;

    void setBusy(bool isBusy);

    Q_INVOKABLE void queryRegisterInfo();

    Q_INVOKABLE void queryRegisterInfoByName(const QString& name);

    Q_INVOKABLE void deleteRegisterInfos(const std::vector<int64_t>& indexIDs);

signals:

    void deleteFinished();
    void deleteError();

    void queryReady(const QVariantList& attendHistory);
    void queryError(const QString& errorMsg);


    void internalQueryRequested();
    void internalQueryRequestedByName(const QString& name);
    void internalDeleteRequested(const std::vector<int64_t>& indexIDs);

    void isBusyChanged();


private:
    QThread* workerThread_;
    RegisterInfoWorker* worker_;
    bool isBusy_=false;
};

class RegisterInfoWorker : public QObject {
    Q_OBJECT

public:
    explicit RegisterInfoWorker(QObject* parent = nullptr);

signals:
    void deleteFinished();
    void deleteError();
    void queryFinished(const QVariantList& attendHistory);
    void queryError(const QString& errorMsg);

public slots:
    void getRegisterInfo();
    void getRegisterInfoByName(const QString& name);
    void deleteRegisterInfos(const std::vector<int64_t>& indexIDs);
};
