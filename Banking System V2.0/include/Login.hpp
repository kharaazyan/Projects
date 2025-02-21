#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <string>
#include <memory>
#include <unistd.h>
#include "DatabaseData.hpp"
#include "DatabaseMeta.hpp"
#include "CubeEncoder.hpp"
#include "AccountMembers.hpp"
#include "EnumClassStatus.hpp"
#include "Utility.hpp"
#include "LogService.hpp"

class Login {
private:
    std::unique_ptr<AccountMembers> member;
    DatabaseData &m_data;
    DatabaseMeta &m_meta;
    CubeEncoder &m_encode;
    LogService &log;

public:
    Login(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService& _log)
        : m_data(data), m_meta(meta), m_encode(encode), log(_log)
    {}

    bool doLogin();  
    void blocked(size_t pos);
    std::unique_ptr<AccountMembers>&& get();
};

#endif
