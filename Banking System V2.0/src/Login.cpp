#include "../include/Login.hpp"

bool Login::doLogin(){
    member = std::make_unique<AccountMembers>();

    std::string tmp;
    Utility::clearScreen();
    std::cout << "Please, enter your username: ";
    std::getline(std::cin, tmp);

    if(m_data.checkInFileAndGetPos(tmp, member->pos)) {
        if(!m_data.getInFile(*member, member->pos)) {
            std::cout << "Failed to read user data!" << std::endl;
            Utility::anyKey();
            return false;
        }
    }
    else {
        std::cout << "User not found!" << std::endl;
        Utility::anyKey();
        return false;
    }

    if(member->account_status == EnumClassStatus::Status::BLOCKED){
        std::cout << "Your account has been blocked!\nContact your system administrator for unlocking!" << std::endl;
        Utility::anyKey();
        return false;
    }

    int count {3};
    std::cout << "Enter your password: ";
    std::getline(std::cin, tmp);

    m_encode = member->passwd; 
    std::string realPasswd = m_encode.decode(member->coding_type);
    realPasswd.erase(realPasswd.find_last_not_of(' ') + 1);

    while(tmp.size() < 8 || tmp.size() > 25){
        std::cout << "Does not fit the length (Minimum: 8 & Maximum 25)\nTry again: ";
        tmp.clear();
        std::getline(std::cin, tmp);
    }


    while(tmp != realPasswd){
        --count;
        if(count == 0){
            blocked(member->pos);
            return false;
        }
        std::cout << "There are " << count << " attempts left. Then the account will be blocked!" << std::endl;
        std::cout << "Try again: ";
        std::getline(std::cin, tmp);
    }

    std::cout << "You have successfully logged in" << std::endl;
    sleep(1);
    return true;
}

void Login::blocked(size_t pos){
    std::string tmp(ACCOUNT_STATUS_SIZE, ' ');
    std::stringstream writeo;
    writeo << tmp;
    m_data.write(writeo, DatabaseData::WriteType::REPLACE, pos + ACCOUNT_STATUS_POS);

    writeo.str("");
    writeo << "Blocked";
    m_data.write(writeo, DatabaseData::WriteType::REPLACE, pos + ACCOUNT_STATUS_POS);

    m_meta.set_blocked(1);
    std::stringstream ss;
    ss << "User [" << member->login << "] was blocked.";
    log.log(ss.str());
    std::cout << "Your account has been blocked due to too many failed attempts!" << std::endl;
    sleep(1);
}

std::unique_ptr<AccountMembers>&& Login::get(){
    return std::move(member);
}
