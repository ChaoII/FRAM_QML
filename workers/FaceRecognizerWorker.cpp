//
// Created by aichao on 2025/9/25.
//

#include "workers/FaceRecognizerWorker.h"
#include <QDateTime>
#include <QRandomGenerator>
#include <QTimer>
#include <QThread>
#include <QDir>
#include "utils.h"
#include "core/dbHelper.h"
#include "core/VectorSearch.h"


FaceRecognizerWorker::FaceRecognizerWorker(QObject* parent): QObject(parent) {
    face_rec_ = std::make_unique<modeldeploy::vision::face::SeetaFaceID>(
        "E:/CLionProjects/ModelDeploy/test_data/test_models/face/face_recognizer.onnx");
}

void FaceRecognizerWorker::processFace(const QImage& image,
                                       std::vector<modeldeploy::vision::DetectionLandmarkResult>& detResult) {
    if (detResult.empty()) return;
    auto mat = Utils::QImageToCvMat(image);
    if (mat.empty()) {
        return;
    }
    cv::Mat rgb_mat;
    cv::cvtColor(mat, rgb_mat, cv::COLOR_RGBA2BGR);
    const auto aligned_images =
        modeldeploy::vision::utils::align_face_with_five_points(modeldeploy::ImageData::from_mat(&rgb_mat), detResult);
    modeldeploy::vision::FaceRecognitionResult result;
    if (!face_rec_->predict(aligned_images[0], &result)) {
        qDebug() << "inference Error";
    }
    // 释放cpu占用,不需要太快的识别速度
    QThread::msleep(100);

    // 向量检索
    const auto searchResult = VectorSearch::getInstance().search(result.embedding, 1);
    const bool isUnknown = searchResult.second.empty() || searchResult.second[0] < 0.62;
    QVariantMap map;
    map["isUnknown"] = isUnknown;

    if (isUnknown) {
        qDebug() << "no staff found";
        map["staffNo"] = "";
        map["name"] = "";
        map["score"] = 0.0;
        map["picUrl"] = "";
        map["attendTime"] = "";
        emit recognizeReady(map);
        return;
    }
    // 通过向量ID查新员工信息
    const auto staffs = DBHelper::getInstance().queryStaffByIndexID(searchResult.first[0]);
    if (staffs.empty()) {
        qWarning() << "[FaceRecognizerWorker] No staff found for index ID:" << searchResult.first[0];
        return;
    }

    const auto& staff = staffs.front();
    const auto attendTime = QDateTime::currentDateTime();
    const QString attendTimeStr = attendTime.toString("yyyy-MM-ddTHH:mm:ss.zzz");


    std::string staffNo = staff.staffNo;
    std::string name = staff.name;
    std::string attendPicUrl = createAttendPicUrl(QString::fromStdString(staffNo)).toStdString();

    // 打印识别结果
    qDebug().noquote() << QString("[FaceRecognizerWorker] Recognized: %1 (%2) score=%3")
                          .arg(name).arg(staffNo).arg(searchResult.second[0]);


    // 封装 QVariantMap 发送到 QML
    map["staffNo"] = QString::fromStdString(staffNo);
    map["name"] = QString::fromStdString(name);
    map["score"] = searchResult.second[0];
    map["picUrl"] = QString::fromStdString(staff.picUrl);
    map["attendTime"] = attendTime;
    emit recognizeReady(map);

    // 缓存打卡信息
    AttendInfo attendInfo;
    attendInfo.name = name;
    attendInfo.staffNo = staffNo;
    attendInfo.attendTime = attendTimeStr.toStdString();
    attendInfo.picUrl = attendPicUrl;
    attendInfos_.emplace_back(attendInfo, image);
}

void FaceRecognizerWorker::saveAttendInfo() {
    if (attendInfos_.empty()) return;
    qDebug() << "[FaceRecognizerWorker] Saving" << attendInfos_.size() << "attend infos...";
    std::unordered_set<std::string> seen;
    std::vector<std::pair<AttendInfo, QImage>> uniqueInfos;
    uniqueInfos.reserve(attendInfos_.size());
    for (const auto& info : attendInfos_) {
        if (seen.insert(info.first.staffNo).second) {
            // .second == true 表示是第一次插入
            uniqueInfos.push_back(info);
        }
    }
    for (const auto& [info,img] : uniqueInfos) {
        const QString picPath = QString::fromStdString(info.picUrl);
        QDir dir = QFileInfo(picPath).absoluteDir();
        if (!dir.exists() && !dir.mkpath(".")) {
            qWarning() << "[FaceRecognizerWorker] Failed to create directory:" << dir.absolutePath();
            continue;
        }

        if (!img.save(picPath)) {
            qWarning() << "[FaceRecognizerWorker] Failed to save image:" << picPath;
            continue;
        }

        if (!DBHelper::getInstance().insertAttendInfo(info)) {
            qWarning() << "[FaceRecognizerWorker] Failed to insert attend info for staffNo:"
                << QString::fromStdString(info.staffNo);
        }
        else {
            qDebug() << "[FaceRecognizerWorker] Saved attend info for staffNo:"
                << QString::fromStdString(info.staffNo);
        }
    }
    attendInfos_.clear();
}

QString FaceRecognizerWorker::createAttendPicUrl(const QString& staffNo) {
    const QString currentMonth = QDateTime::currentDateTime().toString("yyyyMM");
    const QString currentDay = QDateTime::currentDateTime().toString("dd");
    // 每天创建一个目录
    const QString dirPath = "./attend/" + currentMonth + "/" + currentDay;
    const QDir dir(dirPath);
    if (!dir.exists() && !dir.mkpath(dirPath)) {
        qWarning() << "[FaceRecognizerWorker] Failed to create dir:" << dirPath;
    }
    const QString attendPicFileName =
        QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") + "_" + staffNo + ".jpg";
    return dir.filePath(attendPicFileName);
}

