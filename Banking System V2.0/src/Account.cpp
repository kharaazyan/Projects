#include "../include/Account.hpp"

void Account::display() const noexcept {
    Utility::clearScreen();
    std::cout << "Account Information:\n";
    std::cout << "====================\n";
    std::cout << "User: " << login << '\n';
    std::cout << "Account Number: " << number << '\n';
    std::cout << "Balance: $" << std::fixed << std::setprecision(2) << money << '\n';
    std::cout << "User Status: " << statusToString(user_status) << '\n';
    std::cout << "Account Status: " << statusToString(account_status) << '\n';
    std::cout << "====================\n";
    Utility::anyKey();
}

std::stringstream Account::accountToStringstream(){
    std::stringstream result;
    std::string tmp;
    tmp.assign(LINE_SIZE, ' ');

    tmp.replace(LOGIN_POS, login.size(), login);
    tmp.replace(PASSWD_POS, passwd.size(), passwd);
    tmp.replace(CARD_POS, number.size(), number);

    std::string balanceStr = std::to_string(money);
    tmp.replace(MONEY_POS, balanceStr.size(), balanceStr);

    std::string codeStr = std::to_string(coding_type);
    tmp.replace(CODE_POS, codeStr.size(), codeStr);

    std::string userStatusStr = statusToString(user_status);
    tmp.replace(USER_STATUS_POS, userStatusStr.size(), userStatusStr);

    std::string accStatusStr = statusToString(account_status);
    tmp.replace(ACCOUNT_STATUS_POS, accStatusStr.size(), accStatusStr);

    tmp[LINE_SIZE - 1] = '\n';
    result << tmp;
    return result;
}
