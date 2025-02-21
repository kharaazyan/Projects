#include "../include/Registration.hpp" 

std::string RandGeneration::generator(DatabaseData &data){
    srand(time(NULL));
    std::string result;
    do {
        result = "4318 2900 ";
        int count {0};
        for (int i = 0; i < 8; ++i) {
            int digit = ::rand() % 10; 
            result += std::to_string(digit);
            count++;
            if (count == 4 && i != 7) {
                result += ' ';
                count = 0;
            }
        }
    } while(data.checkInFile(result, DatabaseData::CheckType::NUMBER));
    return result;
}

Registration::Registration(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService &_log)
    : m_data(data), m_meta(meta), m_encode(encode), log(_log)
{
    Utility::clearScreen();
    std::cout << "Please enter your username (8..32 symbols): ";
    std::getline(std::cin, login);

    while(m_data.checkInFile(login, DatabaseData::CheckType::LOGIN) 
          || login.size() > 32 
          || login.size() < 8) 
    {
        std::cout << "This username is already occupied or does not fit the length (8..32)!\nTry again: ";
        login.clear();
        std::getline(std::cin, login);
    }

    std::cout << "Please enter your password (8..25 symbols): ";
    std::getline(std::cin, passwd);
    while(passwd.size() < 8 || passwd.size() > 25){
        std::cout << "Does not fit the length (8..25)\nTry again: ";
        passwd.clear();
        std::getline(std::cin, passwd);
    }

    number = generator(m_data);
    m_meta.set_account(1);
    m_meta.set_active(1);
    money = 0;
    account_status = Status::ACTIVE;
    passwd.resize(25, ' ');

    m_encode = passwd;
    passwd = m_encode.encode(coding_type);
    std::stringstream ss;
    ss << "User [" << login << "] registered";
    log.log(ss.str());
}

RegArca::RegArca(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService& _log)
    : Registration(data, meta, encode, _log)
{
    user_status = Status::ARCA;
    meta.set_arca(1);
    m_data.write(accountToStringstream(), DatabaseData::WriteType::END);
    std::cout << "The information has been updated (Arca)!" << std::endl;
    sleep(1);
}

RegVisa::RegVisa(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService& _log)
    : Registration(data, meta, encode, _log)
{
    user_status = Status::VISA;
    meta.set_visa(1);
    m_data.write(accountToStringstream(), DatabaseData::WriteType::END);
    std::cout << "The information has been updated (Visa)!" << std::endl;
    sleep(1);
}

RegPremium::RegPremium(DatabaseData &data, DatabaseMeta &meta, CubeEncoder &encode, LogService& _log)
    : Registration(data, meta, encode, _log)
{
    user_status = Status::PREMIUM;
    meta.set_premium(1);
    m_data.write(accountToStringstream(), DatabaseData::WriteType::END);
    std::cout << "The information has been updated (Premium)!" << std::endl;
    sleep(1);
}
