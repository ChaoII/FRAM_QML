//
// Created by aichao on 2025/10/17.
//

#pragma once
#include <qprocess.h>
#include <QSettings>
#include <QQmlEngine>
#include <QCoreApplication>

class ConfigManager : public QObject {
    Q_OBJECT
    // QML_ELEMENT
    // QML_SINGLETON // 关键声明

    // 所有配置属性
    Q_PROPERTY(bool headlessMode READ headlessMode WRITE setHeadlessMode NOTIFY headlessModeChanged)
    Q_PROPERTY(double detThreshold READ detThreshold WRITE setDetThreshold NOTIFY detThresholdChanged)
    Q_PROPERTY(double recThreshold READ recThreshold WRITE setRecThreshold NOTIFY recThresholdChanged)
    Q_PROPERTY(int faceDBNum READ faceDBNum WRITE setFaceDBNum NOTIFY faceDBNumChanged)
    Q_PROPERTY(int clearOldUnit READ clearOldUnit WRITE setClearOldUnit NOTIFY clearOldUnitChanged)
    Q_PROPERTY(int clearOldInterval READ clearOldInterval WRITE setClearOldInterval NOTIFY clearOldIntervalChanged)
    Q_PROPERTY(
        int attendCacheInterval READ attendCacheInterval WRITE setAttendCacheInterval NOTIFY attendCacheIntervalChanged)
    Q_PROPERTY(int themeMode READ themeMode WRITE setThemeMode NOTIFY themeModeChanged)

public:

    Q_INVOKABLE static void restart() {
        const QString program = QCoreApplication::applicationFilePath();
        const QStringList args = QCoreApplication::arguments();
        QProcess::startDetached(program, args);
        QCoreApplication::quit();
    }


    static ConfigManager* instance() {
        // 永远不销毁只有程序退出时销毁
        static auto* inst = new ConfigManager();
        return inst;
    }

    static QObject* create(QQmlEngine*, QJSEngine*) {
        // QML 引擎拿引用，不负责析构
        return instance();
    }

    // 属性读取
    [[nodiscard]] bool headlessMode() const { return headlessMode_; }
    [[nodiscard]] double detThreshold() const { return detThreshold_; }
    [[nodiscard]] double recThreshold() const { return recThreshold_; }
    [[nodiscard]] int faceDBNum() const { return faceDBNum_; }
    [[nodiscard]] int clearOldUnit() const { return clearOldUnit_; }
    [[nodiscard]] int clearOldInterval() const { return clearOldInterval_; }
    [[nodiscard]] int attendCacheInterval() const { return attendCacheInterval_; }
    [[nodiscard]] int themeMode() const { return themeMode_; }

    // 属性设置
    void setHeadlessMode(bool mode);
    void setDetThreshold(double value);
    void setRecThreshold(double value);
    void setFaceDBNum(int num);
    void setClearOldUnit(int unit);
    void setClearOldInterval(int interval);
    void setAttendCacheInterval(int interval);
    void setThemeMode(int mode);

    // 加载/保存配置
    Q_INVOKABLE void loadConfig();
    Q_INVOKABLE void saveConfig() const;

signals:
    void headlessModeChanged();
    void detThresholdChanged();
    void recThresholdChanged();
    void faceDBNumChanged();
    void clearOldUnitChanged();
    void clearOldIntervalChanged();
    void attendCacheIntervalChanged();
    void themeModeChanged();

private:
    explicit ConfigManager(QObject* parent = nullptr);
    ~ConfigManager() override;



    QSettings* settings_;
    // 配置项默认值
    bool headlessMode_ = false;
    double threshold_ = 0.62;
    double detThreshold_ = 0.9;
    double recThreshold_ = 0.62;
    int faceDBNum_ = 3;
    int clearOldUnit_ = 0; // 0:日 1:月 2年
    int clearOldInterval_ = 30;
    int attendCacheInterval_ = 5;
    int themeMode_ = 0; // 0:浅色 1:深色 2：跟随系统
};
