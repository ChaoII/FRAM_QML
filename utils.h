#pragma once

#include <QImage>
#include <QDebug>
#include <opencv2/opencv.hpp>


class Utils {
public:
    static cv::Mat QImageToCvMat(const QImage& image, bool cloneData = true) {
        switch (image.format()) {
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            cv::Mat mat(image.height(), image.width(), CV_8UC4,
                        const_cast<uchar*>(image.bits()), image.bytesPerLine());
            return cloneData ? mat.clone() : mat;
        }
        case QImage::Format_RGBA8888:
        case QImage::Format_RGBA8888_Premultiplied: {
            // RGBA8888 与 OpenCV 的 CV_8UC4 对应（通道顺序 RGBA）
            cv::Mat mat(image.height(), image.width(), CV_8UC4,
                        const_cast<uchar*>(image.bits()), image.bytesPerLine());
            if (cloneData)
                return mat.clone();
            // 转成 BGR 顺序，OpenCV 默认是 BGR
            cv::Mat bgr;
            cv::cvtColor(mat, bgr, cv::COLOR_RGBA2BGR);
            return bgr;
        }
        case QImage::Format_RGB888: {
            QImage swapped = image.rgbSwapped();
            return cv::Mat(swapped.height(), swapped.width(), CV_8UC3,
                           const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
        }
        case QImage::Format_Grayscale8: {
            return cv::Mat(image.height(), image.width(), CV_8UC1,
                           const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
        }
        default:
            qWarning() << "Unsupported QImage format for conversion:" << image.format();
            return cv::Mat();
        }
    }

    static QImage CvMatToQImage(const cv::Mat& mat) {
        switch (mat.type()) {
        case CV_8UC4: {
            // BGRA → ARGB32
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
            return image.copy(); // 深拷贝
        }
        case CV_8UC3: {
            // BGR → RGB888
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return image.rgbSwapped(); // BGR → RGB
        }
        case CV_8UC1: {
            // 灰度图
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
            return image.copy();
        }
        default:
            qWarning() << "Unsupported cv::Mat format for conversion:" << mat.type();
            return QImage();
        }
    }
};
