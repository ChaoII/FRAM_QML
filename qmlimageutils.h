#pragma once

#include <QObject>
#include <QImage>
#include <QQmlEngine>
#include <QDir>

class QmlImageUtils : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit QmlImageUtils(QObject *parent = nullptr);

    Q_INVOKABLE static QImage cropImage(const QImage &source, const QSize& dstSize);

    Q_INVOKABLE static bool removeFile(const QString& filePath);

    Q_INVOKABLE static bool moveFile(const QString& srcFilePath,const QString& dstFilePath);

    Q_INVOKABLE static void removeEmptyDir(const QDir& dir);

    Q_INVOKABLE static bool saveImage(const QImage &source,const QString& imagePath);

    Q_INVOKABLE static QString getTempPicUrl();

    Q_INVOKABLE static QString createRegisterPicUrl(const QString& staffNo);

    Q_INVOKABLE static QString getTruePicUrlFromTemp(const QString& tempFilePath, const QString& staffNo);


signals:
};

