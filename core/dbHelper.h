//
// Created by aichao on 2025/10/15.
//

#pragma once
#include "schema/schema.h"

inline auto createStorage() {
    auto storage = make_storage("db.sqlite",
                                make_table("staffs",
                                           make_column("id", &Staff::id, primary_key().autoincrement()),
                                           make_column("index_id", &Staff::indexId),
                                           make_column("staff_no", &Staff::staffNo),
                                           make_column("name", &Staff::name),
                                           make_column("register_time", &Staff::registerTime),
                                           make_column("pic_url", &Staff::picUrl)),
                                make_table("attend_infos",
                                           make_column("id", &AttendInfo::id, primary_key().autoincrement()),
                                            make_column("staff_no", &AttendInfo::staffNo),
                                           make_column("name", &AttendInfo::name),
                                           make_column("attend_time", &AttendInfo::attendTime),
                                           make_column("pic_url", &AttendInfo::picUrl)));

    storage.sync_schema();
    return storage;
}

using Storage = decltype(createStorage());

class DBHelper {
public:
    static DBHelper& getInstance() {
        static DBHelper instance;
        return instance;
    }

    static Storage& getStorage() {
        return storage_;
    }

    static bool insertStaff(const Staff& staff);

    static std::vector<Staff> queryStaffs();

    static std::vector<Staff> queryStaffByIndexID(int64_t indexID);

    static bool updateStaff(const Staff& staff);

    static bool insertAttendInfo(const AttendInfo& attendInfo);

    static std::vector<AttendInfo> queryAttendInfos();

private:
    DBHelper();
    static Storage storage_;
};
