#ifndef USER_HPP
#define USER_HPP

#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <limits>
#include <cstdio>

#include "AccountMembers.hpp"
#include "EnumClassStatus.hpp"
#include "DatabaseData.hpp"
#include "DatabaseMeta.hpp"
#include "CubeEncoder.hpp"
#include "Utility.hpp"
#include "LogService.hpp"

class User : public EnumClassStatus {
protected:
    std::unique_ptr<AccountMembers> account;
    DatabaseData &m_data;
    DatabaseMeta &m_meta;
    CubeEncoder &m_encode;
    LogService &m_log;

    std::stringstream toStringstream();

    std::stringstream toStringstream(AccountMembers &member);

    void showBasicUserMenu();    
    void showAdminMenu();

    void viewUserInfo();
    void blockUser();
    void unblockUser();
    void transferForOthers();
    void addMoneyToUser();
    void removeMoneyFromUser();
    void deleteDatabase();

    double getFeePercent() const;

public:
    User(std::unique_ptr<AccountMembers> &&tmp,
         DatabaseData &data,
         DatabaseMeta &meta,
         CubeEncoder &encode,
         LogService &logService);

    virtual ~User();

    void display() const noexcept;
    void blocked();
    void changePassword();
    void checkBalance();
    void depositMoney();
    void withdrawMoney();
    void transferMoney();
    void showTransactionHistory();

    void AccessAdministrator();

    void showMainMenu();
};

#endif
