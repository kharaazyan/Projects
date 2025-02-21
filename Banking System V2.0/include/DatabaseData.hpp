#ifndef DATABASE_DATA_H
#define DATABASE_DATA_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "EnumClassStatus.hpp"
#include "AccountMembers.hpp"
#include "CubeEncoder.hpp"
#include "Utility.hpp"

static const int LINE_BEGIN = 0;
static const int LOGIN_POS = 0;
static const int PASSWD_POS = 33;
static const int CARD_POS = 66;
static const int MONEY_POS = 86;
static const int CODE_POS = 103;
static const int USER_STATUS_POS = 109;
static const int ACCOUNT_STATUS_POS = 118;
static const int LOGIN_SIZE = 32;
static const int PASSWD_SIZE = 32;
static const int PASSWD_SIZE_MAX = 25;
static const int CARD_SIZE = 19;
static const int MONEY_SIZE = 16;
static const int CODE_SIZE = 5;
static const int USER_STATUS_SIZE = 8;
static const int ACCOUNT_STATUS_SIZE = 8;
static const int LINE_SIZE = 127;


class DatabaseData : public EnumClassStatus {
private:
    std::ifstream database_read;
    std::ofstream database_write;

    void first_line();
    void restart();
    std::string trim(const std::string& ptr);
    Status stringToEnum(const std::string &tmp);

public:
    enum class WriteType {
        NONE, END, REPLACE
    };
    enum class CheckType {
        NONE, LOGIN, PASSWD, NUMBER
    };

    DatabaseData();
    ~DatabaseData();

    void write(const std::stringstream& tmp, WriteType type = WriteType::NONE, size_t cur_pos = 0);
    bool checkInFile(std::string &arr, CheckType type = CheckType::NONE);
    bool checkInFileAndGetPos(std::string &arr, size_t &pos);
    bool getInFile(AccountMembers& member, size_t pos);
};

#endif
