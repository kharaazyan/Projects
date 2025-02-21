#include "../include/DatabaseData.hpp"

DatabaseData::DatabaseData() {
    database_write.open("database.txt", std::ios::in | std::ios::out | std::ios::app);
    database_read.open("database.txt");
    if (!database_read.is_open() || !database_write.is_open()) {
        std::cerr << "Error open data file!" << std::endl;
        exit(1);
    }
    database_write.close();
    database_write.open("database.txt", std::ios::in | std::ios::out);

    database_read.seekg(LINE_BEGIN, std::ios::beg);
    if (database_read.tellg() == 0) first_line();
}

DatabaseData::~DatabaseData(){ 
    if(database_read.is_open())  database_read.close();
    if(database_write.is_open()) database_write.close();
}

void DatabaseData::restart(){
    if(database_read.is_open()) {
        database_read.close();
    }
    database_read.open("database.txt");
}

void DatabaseData::first_line() {
    std::string tmp;
    tmp.assign(LINE_SIZE, ' '); 
    tmp.replace(LOGIN_POS, 9, "Username:");
    tmp.replace(PASSWD_POS, 19, "Password by coding:");
    tmp.replace(CARD_POS, 12, "Card number:");
    tmp.replace(MONEY_POS, 9,  "Balance:");
    tmp.replace(CODE_POS, 5,   "Code:");
    tmp.replace(USER_STATUS_POS, 5, "User:");
    tmp.replace(ACCOUNT_STATUS_POS, 8, "Account:");
    tmp += '\n';
    database_write << tmp;
    database_write.flush();
}

void DatabaseData::write(const std::stringstream& tmp, WriteType type, size_t cur_pos){
    switch(type){
        case WriteType::END: {
            database_write.seekp(0, std::ios::end);
            database_write << tmp.str();
            break;
        }
        case WriteType::REPLACE: {
            database_write.seekp(cur_pos, std::ios::beg);
            database_write << tmp.str();
            break;
        }
        default: 
            std::cerr << "Unknown command!" << std::endl; 
            exit(1);
    }
    database_write.flush();
}

bool DatabaseData::checkInFile(std::string &arr, CheckType type){
    if(arr.size() < 1) return true; 
    restart();

    database_read.seekg(LINE_SIZE, std::ios::beg); 
    std::string readLine;
    std::string tmp;
    while (std::getline(database_read, readLine)) {
        switch(type) {
            case CheckType::LOGIN: {
                tmp = readLine.substr(LOGIN_POS, LOGIN_SIZE);
                if(trim(tmp) == arr) return true;
                else break;
            }
            case CheckType::PASSWD: {
                tmp = readLine.substr(PASSWD_POS, PASSWD_SIZE);
                if(trim(tmp) == arr) return true;
                else break;
            }
            case CheckType::NUMBER: {
                tmp = readLine.substr(CARD_POS, CARD_SIZE);
                if(trim(tmp) == arr) return true;
                else break;
            }
            default: {
                std::cerr << "Unknown CheckType!" << std::endl;
                exit(1);
            }
        }
    }
    return false;
}

bool DatabaseData::checkInFileAndGetPos(std::string &arr, size_t &pos){
    std::string readLine;
    std::string tmp;
    restart();
    database_read.seekg(LINE_SIZE, std::ios::beg); 
    while (std::getline(database_read, readLine)) {
        pos = database_read.tellg();
        pos -= LINE_SIZE;
        tmp = readLine.substr(LOGIN_POS, LOGIN_SIZE);
        if(trim(tmp) == arr) return true;
    }
    return false;
}

bool DatabaseData::getInFile(AccountMembers& member, size_t position){
    std::string tmp;
    restart();
    database_read.seekg(position, std::ios::beg);

    if (!std::getline(database_read, tmp)) {
        return false;
    }
    member.login  = trim(tmp.substr(LOGIN_POS, LOGIN_SIZE));
    member.passwd = tmp.substr(PASSWD_POS, PASSWD_SIZE_MAX);
    member.number = trim(tmp.substr(CARD_POS, CARD_SIZE));
    member.money  = Utility::myatoi(tmp.substr(MONEY_POS, MONEY_SIZE));
    member.coding_type = Utility:: myatoi(tmp.substr(CODE_POS, CODE_SIZE));
    member.user_status    = stringToEnum(trim(tmp.substr(USER_STATUS_POS, USER_STATUS_SIZE)));
    member.account_status = stringToEnum(trim(tmp.substr(ACCOUNT_STATUS_POS, ACCOUNT_STATUS_SIZE)));
    return true;
}

DatabaseData::Status DatabaseData::stringToEnum(const std::string &value){
    if(value == "Arca")     return Status::ARCA;
    if(value == "Visa")     return Status::VISA;
    if(value == "Premium")  return Status::PREMIUM;
    if(value == "Blocked")  return Status::BLOCKED;
    if(value == "Active")   return Status::ACTIVE;
    if(value == "Inactive") return Status::INACTIVE;
    if(value == "Moder")return Status::MODER;
    if(value == "Admin")    return Status::ADMIN;
    return Status::NONE;
}

std::string DatabaseData::trim(const std::string& ptr){
    if(ptr.empty()) return ptr;
    size_t first = 0;
    while (first < ptr.size() && ptr[first] == ' ') { ++first; }
    size_t last = ptr.size() - 1;
    while (last > 0 && ptr[last] == ' ') { --last; }
    return ptr.substr(first, (last - first + 1));
}
