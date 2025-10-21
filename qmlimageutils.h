#pragma once

#include <QObject>
#include <QImage>
#include <QQmlEngine>

class QmlImageUtils : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit QmlImageUtils(QObject *parent = nullptr);

    Q_INVOKABLE static QImage cropImage(const QImage &source, const QSize& dstSize);

    Q_INVOKABLE static bool removeFile(const QString& filePath);

    Q_INVOKABLE static bool saveImage(const QImage &source,const QString& imagePath);

    Q_INVOKABLE static QString createRegisterPicUrl(const QString& staffNo);

signals:
};

