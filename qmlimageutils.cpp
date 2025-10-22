#include "qmlimageutils.h"

#include <QDir>

QmlImageUtils::QmlImageUtils(QObject *parent)
    : QObject{parent}
{}

bool QmlImageUtils::saveImage(const QImage &source,const QString& imagePath){
    return source.save(imagePath);
}

bool QmlImageUtils::removeFile(const QString& filePath){
    if (filePath.isEmpty() || !QFile::exists(filePath)) {
        qWarning() << "文件不存在或路径为空:" << filePath;
        return false;
    }
    QFileInfo fileInfo(filePath);
    auto dir = fileInfo.dir();
    QFile file(filePath);
    if (!file.remove()) {
        qWarning() << "删除失败:" << file.errorString();
        return false;
    }
    // 如果文件夹为空，那么删除文件夹
    if(dir.isEmpty()){
        bool ret = dir.rmdir(dir.absolutePath());
        qDebug()<<"dir: "<<dir.absolutePath()<<" is empty, remove it: " << ret;
        return ret;
    }
    return true;
}

bool QmlImageUtils::moveFile(const QString& srcFilePath,const QString& dstFilePath){
    return  QFile::rename(srcFilePath, dstFilePath);
}


void QmlImageUtils::removeEmptyDir(const QDir& dir){
    // 检查目录是否存在
    if (dir.exists() && dir.isEmpty()) {
        dir.rmdir(dir.absolutePath());
    }
}

QString QmlImageUtils::getTempPicUrl(){
    QString fileName = "temp_register_" +
                       QDateTime::currentDateTime()
                           .toString("yyyyMMddhhmmsszzz") + ".jpg";
    auto tempDir = QDir::temp();
    QString tempFilePath = tempDir.absoluteFilePath(fileName);
    return tempFilePath;
}


QString QmlImageUtils::createRegisterPicUrl(const QString& staffNo) {
    // 1. 定义目标目录（这里用 "./face"，但推荐用绝对路径）
    const QString dirPath = "./face/"+staffNo; // 建议替换成配置文件或环境变量中的路径
    // 2. 获取规范化后的绝对路径（避免 "./face" 被解析成嵌套路径）
    QDir absoluteDir(QDir(dirPath).absolutePath()); // 关键点：先转绝对路径
    // 3. 创建目录（如果不存在）
    if (!absoluteDir.exists() && !QDir().mkpath(absoluteDir.absolutePath())) {
        qWarning() << "[FaceRecognizerWorker] Failed to create dir:" << absoluteDir.absolutePath();
        return "";
    }
    // 4. 生成文件名（带时间戳和员工ID）
    const QString fileName =
        QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") +
        "_" + staffNo + ".jpg";
    // 5. 返回绝对路径（格式如 /project_root/face/20231010120000_1001.jpg）
    return absoluteDir.absoluteFilePath(fileName);
}


QString QmlImageUtils::getTruePicUrlFromTemp(const QString& tempFilePath, const QString& staffNo){

    QFileInfo fileInfo(tempFilePath);
    QString name = fileInfo.fileName();
    QString newFilename = name.replace("temp_register_","");
    const QString dirPath = "./face/"+staffNo; // 建议替换成配置文件或环境变量中的路径
    // 2. 获取规范化后的绝对路径（避免 "./face" 被解析成嵌套路径）
    QDir absoluteDir(QDir(dirPath).absolutePath()); // 关键点：先转绝对路径
    // 3. 创建目录（如果不存在）
    if (!absoluteDir.exists() && !QDir().mkpath(absoluteDir.absolutePath())) {
        qWarning() << "[FaceRecognizerWorker] Failed to create dir:" << absoluteDir.absolutePath();
        return "";
    }
    return absoluteDir.absoluteFilePath(newFilename);
}


QImage QmlImageUtils::cropImage(const QImage &source, const QSize& dstSize) {
    const QSize originalSize = source.size();
    if (originalSize.isEmpty()) {
        qWarning() << "Original image is empty";
        return{};
    }
    // 计算缩放比例（保持宽高比并填充）
    const double scaleX = static_cast<double>(dstSize.width()) / originalSize.width();
    const double scaleY = static_cast<double>(dstSize.height()) / originalSize.height();
    const double scale = qMax(scaleX, scaleY); // PreserveAspectCrop 使用最大值

    // 计算缩放后的尺寸
    const QSize scaledSize(qRound(originalSize.width() * scale),
                           qRound(originalSize.height() * scale));

    // 计算裁剪区域（居中裁剪）
    const QRect cropRect((scaledSize.width() - dstSize.width()) / 2,
                         (scaledSize.height() - dstSize.height()) / 2,
                         dstSize.width(), dstSize.height());

    // 先缩放到目标尺寸，然后裁剪
    const QImage scaledImage = source.scaled(
        scaledSize,
        Qt::KeepAspectRatioByExpanding, // 关键：保持比例并扩展
        Qt::SmoothTransformation);
    // 裁剪到 320x600
    const QImage croppedImage = scaledImage.copy(cropRect);
    return croppedImage;
}
