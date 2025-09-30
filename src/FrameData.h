//
// Created by aichao on 2025/9/30.
//

#pragma once

#include <opencv2/opencv.hpp>

struct FrameData {
    cv::Mat frame;
    qint64 timestamp;
    int sequence;
    
    FrameData() : timestamp(0), sequence(0) {}
    
    FrameData(cv::Mat&& f, qint64 ts, int seq) 
        : frame(std::move(f)), timestamp(ts), sequence(seq) {}
        
    FrameData(const cv::Mat& f, qint64 ts, int seq) 
        : frame(f), timestamp(ts), sequence(seq) {}
};