#ifndef ACCOUNT_MEMBERS_H
#define ACCOUNT_MEMBERS_H

#include <string>
#include "EnumClassStatus.hpp"

class AccountMembers : public EnumClassStatus {
public:
    std::string login;
    std::string passwd;
    std::string number;
    int money {0};
    int coding_type {0};
    Status user_status {Status::NONE};
    Status account_status {Status::NONE};
    size_t pos {0};
};

#endif
