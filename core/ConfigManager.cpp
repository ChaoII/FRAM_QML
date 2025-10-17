//
// Created by aichao on 2025/10/17.
//

#include "core/ConfigManager.h"

ConfigManager::ConfigManager(QObject* parent) : QObject(parent) {
    // 会自动析构
    settings_ = new QSettings("config.ini", QSettings::IniFormat,this);
    loadConfig(); // 自动加载配置
}

ConfigManager::~ConfigManager() {
    saveConfig(); // 自动保存
}

void ConfigManager::setHeadlessMode(bool mode) {
    if (headlessMode_ != mode) {
        headlessMode_ = mode;
        emit headlessModeChanged();
    }
}


void ConfigManager::setDetThreshold(const double value) {
    if (value != detThreshold_) {
        detThreshold_ = value;
        emit detThresholdChanged();
    }
}

void ConfigManager::setRecThreshold(const double value) {
    if (value != recThreshold_) {
        recThreshold_ = value;
        emit recThresholdChanged();
    }
}

void ConfigManager::setFaceDBNum(const int num) {
    if (faceDBNum_ != num) {
        faceDBNum_ = num;
        emit faceDBNumChanged();
    }
}

void ConfigManager::setClearOldUnit(const int unit) {
    if (clearOldUnit_ != unit) {
        clearOldUnit_ = unit;
        emit clearOldUnitChanged();
    }
}

void ConfigManager::setClearOldInterval(const int interval) {
    if (clearOldInterval_ != interval) {
        clearOldInterval_ = interval;
        emit clearOldIntervalChanged();
    }
}

void ConfigManager::setAttendCacheInterval(const int interval) {
    if (attendCacheInterval_ != interval) {
        attendCacheInterval_ = interval;
        emit attendCacheIntervalChanged();
    }
}

void ConfigManager::setThemeMode(const int mode) {
    if (themeMode_ != mode) {
        themeMode_ = mode;
        emit themeModeChanged();
    }
}


// 加载所有配置
void ConfigManager::loadConfig() {
    settings_->sync();
    headlessMode_ = settings_->value("UI/HeadlessMode", false).toBool();
    detThreshold_ = settings_->value("Global/DetThreshold", 0.9).toDouble();
    recThreshold_ = settings_->value("Global/RecThreshold", 0.62).toDouble();
    faceDBNum_ = settings_->value("Global/MaxFaces", 3).toInt();
    clearOldUnit_ = settings_->value("Global/Unit", 0).toInt();
    clearOldInterval_ = settings_->value("Global/Interval", 30).toInt();
    attendCacheInterval_ = settings_->value("Global/CacheInterval", 5).toInt();
    themeMode_ = settings_->value("UI/Theme", 0).toInt();
}

// 保存所有配置
void ConfigManager::saveConfig() const {
    settings_->setValue("UI/HeadlessMode", headlessMode_);
    settings_->setValue("Global/DetThreshold", detThreshold_);
    settings_->setValue("Global/RecThreshold", recThreshold_);
    settings_->setValue("Global/MaxFaces", faceDBNum_);
    settings_->setValue("Global/Unit", clearOldUnit_);
    settings_->setValue("Global/Interval", clearOldInterval_);
    settings_->setValue("Global/CacheInterval", attendCacheInterval_);
    settings_->setValue("UI/Theme", themeMode_);
    settings_->sync();
}
