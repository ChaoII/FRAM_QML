//
// Created by aichao on 2025/10/15.
//

#include "core/dbHelper.h"
#include <QDebug>


Storage DBHelper::storage_ = createStorage();


DBHelper::DBHelper() {
    storage_.sync_schema();
}

bool DBHelper::insertStaff(const Staff& staff) {
    try {
        storage_.insert(staff);
        return true;
    }
    catch (const std::exception& e) {
        qDebug() << "insert staff failed: " << e.what();
        return false;
    }
}

std::vector<Staff> DBHelper::queryStaffs() {
    try {
        return storage_.get_all<Staff>();;
    }
    catch (const std::exception& e) {
        qDebug() << "query staff failed: " << e.what();
        return {};
    }
}

std::vector<Staff> DBHelper::queryStaffByIndexID(const int64_t indexID) {
    try {
        auto staffs = storage_.get_all<Staff>(where(c(&Staff::indexId) == indexID));
        return staffs;
    }
    catch (const std::exception& e) {
        qDebug() << "query staff failed, indexID is: " << indexID << "error :" << e.what();
        return {};
    }
}

bool DBHelper::updateStaff(const Staff& staff) {
    try {
        storage_.update(staff);
        return true;
    }
    catch (const std::exception& e) {
        qDebug() << "update staff failed: " << e.what();
        return false;
    }
}


bool DBHelper::insertAttendInfo(const AttendInfo& attendInfo) {
    try {
        storage_.insert(attendInfo);
        return true;
    }
    catch (const std::exception& e) {
        qDebug() << "insert attend_info failed: " << e.what();
        return false;
    }
}


std::vector<AttendInfo> DBHelper::queryAttendInfos() {
    try {
        return storage_.get_all<AttendInfo>();;
    }
    catch (const std::exception& e) {
        qDebug() << "query Attend_info failed: " << e.what();
        return {};
    }
}
