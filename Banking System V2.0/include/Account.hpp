#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "AccountMembers.hpp"
#include "DatabaseData.hpp"
#include "Utility.hpp"
#include "CubeEncoder.hpp"

class Account : public EnumClassStatus {
public:
    virtual ~Account() = default;
    void display() const noexcept;

protected:
    std::string login;
    std::string passwd;
    std::string number;
    int money;
    int coding_type {0};
    Status user_status {Status::NONE};
    Status account_status {Status::NONE};
    size_t pos {0};

    std::stringstream accountToStringstream();
};

#endif
