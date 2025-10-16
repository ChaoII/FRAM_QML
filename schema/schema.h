//
// Created by aichao on 2025/10/15.
//

#pragma once

#include <string>
#include <memory>
#include "core/sqlite_orm.h"
using namespace sqlite_orm;

struct Staff {
    int id;
    int64_t indexId;
    std::string staffNo;
    std::string name;
    std::string registerTime;
    std::string picUrl;
};

struct AttendInfo {
    int id;
    std::string staffNo;
    std::string name;
    std::string attendTime;
    std::string picUrl;
};





