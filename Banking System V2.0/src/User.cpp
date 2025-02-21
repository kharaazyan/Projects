#include "../include/User.hpp"

User::User(std::unique_ptr<AccountMembers> &&tmp,
           DatabaseData &data,
           DatabaseMeta &meta,
           CubeEncoder &encode,
           LogService &logService)
    : account(std::move(tmp)), 
      m_data(data), 
      m_meta(meta), 
      m_encode(encode),
      m_log(logService)
{
    std::stringstream ss;
    ss << "User [" << account->login << "] logged in.";
    m_log.log(ss.str());
}

User::~User() {
    m_data.write(toStringstream(), DatabaseData::WriteType::REPLACE, account->pos);

    std::stringstream ss;
    ss << "User [" << account->login << "] logged out.";
    m_log.log(ss.str());
}

void User::display() const noexcept {
    Utility::clearScreen();
    std::cout << "Account Information:\n";
    std::cout << "====================\n";
    std::cout << "User: " << account->login << '\n';
    std::cout << "Card Number: " << account->number << '\n';
    std::cout << "Balance: $" << std::fixed << std::setprecision(2) << account->money << '\n';
    std::cout << "User Status: " << statusToString(account->user_status) << '\n';
    std::cout << "Account Status: " << statusToString(account->account_status) << '\n';
    std::cout << "====================\n";
    Utility::anyKey();
}

void User::blocked() {
    Utility::clearScreen();
    std::cout << "Are you sure you want to block YOUR OWN account? (Y/N): ";
    char c;
    std::cin >> c;
    if(std::tolower(c) != 'y') {
        std::cout << "Block canceled.\n";
        Utility::anyKey();
        return;
    }
    account->account_status = Status::BLOCKED;
    m_meta.set_blocked(1);
    m_data.write(toStringstream(), DatabaseData::WriteType::REPLACE, account->pos);

    std::stringstream ss;
    ss << "User [" << account->login << "] has been BLOCKED.";
    m_log.log(ss.str());

    std::cout << "Your account has been blocked!\n";
    Utility::anyKey();
}

void User::changePassword() {
    Utility::clearScreen();
    std::cout << "Enter new password (8-25 characters): ";
    std::string newPass;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, newPass);

    while(newPass.size() < 8 || newPass.size() > 25) {
        std::cout << "Incorrect size. Try again: ";
        newPass.clear();
        std::getline(std::cin, newPass);
    }
    newPass.resize(25, ' ');

    m_encode = newPass;
    newPass = m_encode.encode(account->coding_type);
    account->passwd = newPass;

    m_data.write(toStringstream(), DatabaseData::WriteType::REPLACE, account->pos);

    std::stringstream ss;
    ss << "User [" << account->login << "] changed password.";
    m_log.log(ss.str());

    std::cout << "Password changed successfully!\n";
    Utility::anyKey();
}

void User::checkBalance() {
    Utility::clearScreen();
    std::cout << "Your current balance is $"
              << std::fixed << std::setprecision(2) 
              << account->money << std::endl;
    Utility::anyKey();
}

void User::depositMoney() {
    Utility::clearScreen();
    std::cout << "Enter amount to deposit: $";
    double amount;
    std::cin >> amount;

    while(std::cin.fail() || amount <= 0) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid amount! Try again: $";
        std::cin >> amount;
    }
    account->money += amount;
    m_data.write(toStringstream(), DatabaseData::WriteType::REPLACE, account->pos);
    m_meta.set_money(amount);
    m_meta.set_transaction(1);

    std::stringstream ss;
    ss << "User [" << account->login << "] deposited $" << amount 
       << "; new balance: $" << account->money;
    m_log.log(ss.str());

    std::cout << "Deposit successful!\n";
    Utility::anyKey();
}

void User::withdrawMoney() {
    Utility::clearScreen();
    std::cout << "Enter amount to withdraw: $";
    double amount;
    std::cin >> amount;

    while(std::cin.fail() || amount <= 0) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid amount! Try again: $";
        std::cin >> amount;
    }

    if(amount > account->money) {
        std::cout << "Insufficient funds!\n";
        Utility::anyKey();
        return;
    }

    double fee = amount * getFeePercent();
    double total = amount + fee;

    if(total > account->money) {
        std::cout << "Insufficient funds (including fee)!\n";
        Utility::anyKey();
        return;
    }

    account->money -= total;
    m_data.write(toStringstream(), DatabaseData::WriteType::REPLACE, account->pos);
    m_meta.set_money(-total);
    m_meta.set_transaction(1);

    std::stringstream ss;
    ss << "User [" << account->login << "] withdrew $" << amount
       << " with fee $" << fee << ", new balance: $" << account->money;
    m_log.log(ss.str());

    std::cout << "Withdraw successful! Fee: $" << fee << "\n";
    Utility::anyKey();
}

void User::transferMoney() {
    Utility::clearScreen();
    std::cout << "Enter username (recipient): ";
    std::string recipient;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::getline(std::cin, recipient);

    AccountMembers other;
    if(!m_data.checkInFileAndGetPos(recipient, other.pos)) {
        std::cout << "User not found!\n";
        sleep(1);
        return;
    }

    if(!m_data.getInFile(other, other.pos)) {
        std::cout << "Failed to read user data!\n";
        Utility::anyKey();
        return;
    }

    std::cout << "Enter amount to transfer: $";
    double amount;
    std::cin >> amount;
    while(std::cin.fail() || amount <= 0) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid amount! Try again: $";
        std::cin >> amount;
    }

    if(amount > account->money) {
        std::cout << "Insufficient funds!\n";
        Utility::anyKey();
        return;
    }

    double fee = amount * getFeePercent();
    double total = amount + fee;

    if(total > account->money) {
        std::cout << "Insufficient funds (including fee)!\n";
        Utility::anyKey();
        return;
    }

    account->money -= total;
    other.money += amount;
    m_meta.set_transaction(1);

    m_data.write(toStringstream(), DatabaseData::WriteType::REPLACE, account->pos);
    m_data.write(toStringstream(other), DatabaseData::WriteType::REPLACE, other.pos);

    std::stringstream ss;
    ss << "User [" << account->login << "] transferred $"
       << amount << " with fee $" << fee << " to user [" << recipient
       << "]. New balance: $" << account->money;
    m_log.log(ss.str());

    std::cout << "Transfer successful! Fee: $" << fee << "\n";
    Utility::anyKey();
}

void User::showTransactionHistory() {
    Utility::clearScreen();
    std::cout << "------- LOG HISTORY -------\n";
    m_log.showLog();
    Utility::anyKey();
}


void User::AccessAdministrator() {
    Utility::clearScreen();
    std::cout << "Enter secret code for becoming Admin: ";
    std::string code;
    std::cin >> code;
    if (code != "admin777") {
        std::cout << "Wrong code!\n";
        Utility::anyKey();
        return;
    }
    account->user_status = Status::ADMIN;
    m_meta.set_admin(1);
    m_data.write(toStringstream(), DatabaseData::WriteType::REPLACE, account->pos);

    std::stringstream ss;
    ss << "User [" << account->login << "] is now Admin.";
    m_log.log(ss.str());

    std::cout << "Now you are Administrator!\n";
    Utility::anyKey();
}

void User::showMainMenu() {
    switch (account->user_status) {
    case Status::ADMIN:
        showAdminMenu();
        break;
    default:
        showBasicUserMenu();
    }
}

void User::showBasicUserMenu() {
    while(true) {
        Utility::clearScreen();
        std::cout << "========== Basic User Menu (ACTIVE) ==========\n";
        std::cout << "1. Display Account Info\n";
        std::cout << "2. Check Balance\n";
        std::cout << "3. Deposit Money\n";
        std::cout << "4. Withdraw Money\n";
        std::cout << "5. Transfer Money\n";
        std::cout << "6. Show Log History\n";
        std::cout << "7. Change Password\n";
        std::cout << "8. Block Account\n";
        std::cout << "9. Become Admin\n";
        std::cout << "0. Exit\n";
        int choice;
        std::cin >> choice;
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        switch(choice) {
            case 1: display(); break;
            case 2: checkBalance(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: transferMoney(); break;
            case 6: showTransactionHistory(); break;
            case 7: changePassword(); break;
            case 8: blocked(); return;
            case 9: AccessAdministrator(); return;
            case 0:
                std::cout << "Exiting user menu...\n";
                sleep(1);
                return;
            default:
                std::cout << "Invalid choice.\n";
                sleep(1);
                break;
        }
    }
}


void User::showAdminMenu() {
    while(true) {
        Utility::clearScreen();
        std::cout << "=============== Admin Menu ===============\n";
        std::cout << "1. View info about user (by login)\n";
        std::cout << "2. Block user (by login)\n";
        std::cout << "3. Unblock user (by login)\n";
        std::cout << "4. Transfer money between two users (by login)\n";
        std::cout << "5. Add money to user (by login)\n";
        std::cout << "6. Remove money from user (by login)\n";
        std::cout << "7. Delete entire database (except log.txt)\n";
        std::cout << "8. Show Log History\n";
        std::cout << "0. Exit\n";
        int choice;
        std::cin >> choice;
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        switch(choice) {
            case 1: viewUserInfo(); break;
            case 2: blockUser(); break;
            case 3: unblockUser(); break;
            case 4: transferForOthers(); break;
            case 5: addMoneyToUser(); break;
            case 6: removeMoneyFromUser(); break;
            case 7: deleteDatabase(); break;
            case 8: showTransactionHistory(); break;
            case 0:
                std::cout << "Exiting admin menu...\n";
                sleep(1);
                return;
            default:
                std::cout << "Invalid choice.\n";
                sleep(1);
                break;
        }
    }
}

void User::viewUserInfo() {
    Utility::clearScreen();
    std::cout << "Enter login: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string search;
    std::getline(std::cin, search);

    AccountMembers tmp;
    if(!m_data.checkInFileAndGetPos(search, tmp.pos)) {
        std::cout << "User not found.\n";
        Utility::anyKey();
        return;
    }
    if(!m_data.getInFile(tmp, tmp.pos)) {
        std::cout << "Failed to read user data.\n";
        Utility::anyKey();
        return;
    }
    Utility::clearScreen();
    std::cout << "User: " << tmp.login << "\n";
    std::cout << "Card: " << tmp.number << "\n";
    std::cout << "Balance: $" << tmp.money << "\n";
    std::cout << "User Status: " << statusToString(tmp.user_status) << "\n";
    std::cout << "Account Status: " << statusToString(tmp.account_status) << "\n";
    Utility::anyKey();
}

void User::blockUser() {
    Utility::clearScreen();
    std::cout << "Enter login to block: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string s;
    std::getline(std::cin, s);

    AccountMembers tmp;
    if(!m_data.checkInFileAndGetPos(s, tmp.pos)) {
        std::cout << "User not found!\n";
        Utility::anyKey();
        return;
    }
    if(!m_data.getInFile(tmp, tmp.pos)) {
        std::cout << "Failed to read!\n";
        Utility::anyKey();
        return;
    }
    tmp.account_status = Status::BLOCKED;
    m_meta.set_blocked(1);
    m_data.write(toStringstream(tmp), DatabaseData::WriteType::REPLACE, tmp.pos);
    std::stringstream ss;
    ss << "[ADMIN " << account->login << "] blocked user [" << tmp.login << "].";
    m_log.log(ss.str());
    std::cout << "User blocked.\n";
    Utility::anyKey();
}

void User::unblockUser() {
    Utility::clearScreen();
    std::cout << "Enter login to unblock: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string s;
    std::getline(std::cin, s);

    AccountMembers tmp;
    if(!m_data.checkInFileAndGetPos(s, tmp.pos)) {
        std::cout << "User not found!\n";
        Utility::anyKey();
        return;
    }
    if(!m_data.getInFile(tmp, tmp.pos)) {
        std::cout << "Failed to read!\n";
        Utility::anyKey();
        return;
    }
    if(tmp.account_status != Status::BLOCKED) {
        std::cout << "User not blocked.\n";
        Utility::anyKey();
        return;
    }
    tmp.account_status = Status::ACTIVE;
    m_data.write(toStringstream(tmp), DatabaseData::WriteType::REPLACE, tmp.pos);
    std::stringstream ss;
    ss << "[MOD/ADMIN " << account->login << "] unblocked user [" << tmp.login << "].";
    m_log.log(ss.str());
    std::cout << "User unblocked.\n";
    Utility::anyKey();
}

void User::transferForOthers() {
    Utility::clearScreen();
    std::cout << "Enter source user (login): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string src;
    std::getline(std::cin, src);
    AccountMembers sUser;
    if(!m_data.checkInFileAndGetPos(src, sUser.pos)) {
        std::cout << "Source user not found!\n";
        Utility::anyKey();
        return;
    }
    if(!m_data.getInFile(sUser, sUser.pos)) {
        std::cout << "Failed to read source user!\n";
        Utility::anyKey();
        return;
    }
    std::cout << "Enter destination user (login): ";
    std::string dst;
    std::getline(std::cin, dst);
    AccountMembers dUser;
    if(!m_data.checkInFileAndGetPos(dst, dUser.pos)) {
        std::cout << "Destination user not found!\n";
        Utility::anyKey();
        return;
    }
    if(!m_data.getInFile(dUser, dUser.pos)) {
        std::cout << "Failed to read dest user!\n";
        Utility::anyKey();
        return;
    }
    std::cout << "Enter amount to transfer: $";
    double amount;
    std::cin >> amount;
    while(std::cin.fail() || amount <= 0) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid amount! Try again: $";
        std::cin >> amount;
    }
    if(amount > sUser.money) {
        std::cout << "Insufficient funds in source user!\n";
        Utility::anyKey();
        return;
    }
    sUser.money -= amount;
    dUser.money += amount;
    m_meta.set_transaction(1);
    m_data.write(toStringstream(sUser), DatabaseData::WriteType::REPLACE, sUser.pos);
    m_data.write(toStringstream(dUser), DatabaseData::WriteType::REPLACE, dUser.pos);
    std::stringstream ss;
    ss << "[MOD/ADMIN " << account->login << "] transferred $"
       << amount << " from [" << sUser.login << "] to [" << dUser.login << "].";
    m_log.log(ss.str());
    std::cout << "Transfer successful.\n";
    Utility::anyKey();
}

void User::addMoneyToUser() {
    Utility::clearScreen();
    std::cout << "Enter login: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string s;
    std::getline(std::cin, s);

    AccountMembers tmp;
    if(!m_data.checkInFileAndGetPos(s, tmp.pos)) {
        std::cout << "User not found!\n";
        Utility::anyKey();
        return;
    }
    if(!m_data.getInFile(tmp, tmp.pos)) {
        std::cout << "Failed to read user data.\n";
        Utility::anyKey();
        return;
    }
    std::cout << "Enter amount to add: $";
    double amount;
    std::cin >> amount;
    while(std::cin.fail() || amount <= 0) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid amount! Try again: $";
        std::cin >> amount;
    }
    tmp.money += amount;
    m_meta.set_money(amount);
    m_data.write(toStringstream(tmp), DatabaseData::WriteType::REPLACE, tmp.pos);
    std::stringstream ss;
    ss << "[ADMIN " << account->login << "] added $" << amount 
       << " to user [" << tmp.login << "]. New bal = $" << tmp.money;
    m_log.log(ss.str());
    std::cout << "Added money successfully!\n";
    Utility::anyKey();
}

void User::removeMoneyFromUser() {
    Utility::clearScreen();
    std::cout << "Enter login: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string s;
    std::getline(std::cin, s);

    AccountMembers tmp;
    if(!m_data.checkInFileAndGetPos(s, tmp.pos)) {
        std::cout << "User not found!\n";
        Utility::anyKey();
        return;
    }
    if(!m_data.getInFile(tmp, tmp.pos)) {
        std::cout << "Failed to read user data.\n";
        Utility::anyKey();
        return;
    }
    std::cout << "Enter amount to remove: $";
    double amount;
    std::cin >> amount;
    while(std::cin.fail() || amount <= 0) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid amount! Try again: $";
        std::cin >> amount;
    }
    if(amount > tmp.money) {
        std::cout << "That user doesn't have enough!\n";
        Utility::anyKey();
        return;
    }
    tmp.money -= amount;
    m_meta.set_money(-amount);
    m_data.write(toStringstream(tmp), DatabaseData::WriteType::REPLACE, tmp.pos);
    std::stringstream ss;
    ss << "[ADMIN " << account->login << "] removed $" << amount
       << " from user [" << tmp.login << "]. New bal = $" << tmp.money;
    m_log.log(ss.str());
    std::cout << "Removed money successfully!\n";
    Utility::anyKey();
}

void User::deleteDatabase() {
    Utility::clearScreen();
    std::cout << "WARNING! This will delete database.txt and metainfo.txt.\n";
    std::cout << "log.txt will remain.\n";
    std::cout << "Enter admin password to confirm: ";
    std::string pass;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, pass);
    if(pass != "root123") {
        std::cout << "Wrong password.\n";
        Utility::anyKey();
        return;
    }
    std::remove("database.txt");
    std::remove("metainfo.txt");
    std::stringstream ss;
    ss << "[ADMIN " << account->login << "] deleted entire DB (except log.txt).";
    m_log.log(ss.str());
    std::cout << "Database deleted!\n";
    Utility::anyKey();
    exit(0);

}

std::stringstream User::toStringstream() {
    std::stringstream result;
    std::string tmp;
    tmp.assign(LINE_SIZE, ' ');
    tmp.replace(LOGIN_POS, account->login.size(), account->login);
    tmp.replace(PASSWD_POS, account->passwd.size(), account->passwd);
    tmp.replace(CARD_POS, account->number.size(), account->number);

    std::string str = std::to_string(static_cast<int>(account->money));
    tmp.replace(MONEY_POS, str.size(), str);

    str = std::to_string(account->coding_type);
    tmp.replace(CODE_POS, str.size(), str);

    str = statusToString(account->user_status);
    tmp.replace(USER_STATUS_POS, str.size(), str);

    str = statusToString(account->account_status);
    tmp.replace(ACCOUNT_STATUS_POS, str.size(), str);

    tmp[LINE_SIZE - 1] = '\n';
    result << tmp;
    return result;
}

std::stringstream User::toStringstream(AccountMembers &member) {
    std::stringstream result;
    std::string tmp;
    tmp.assign(LINE_SIZE, ' ');
    tmp.replace(LOGIN_POS, member.login.size(), member.login);
    tmp.replace(PASSWD_POS, member.passwd.size(), member.passwd);
    tmp.replace(CARD_POS, member.number.size(), member.number);

    std::string str = std::to_string(static_cast<int>(member.money));
    tmp.replace(MONEY_POS, str.size(), str);

    str = std::to_string(member.coding_type);
    tmp.replace(CODE_POS, str.size(), str);

    str = statusToString(member.user_status);
    tmp.replace(USER_STATUS_POS, str.size(), str);

    str = statusToString(member.account_status);
    tmp.replace(ACCOUNT_STATUS_POS, str.size(), str);

    tmp[LINE_SIZE - 1] = '\n';
    result << tmp;
    return result;
}

double User::getFeePercent() const {
    switch (account->user_status) {
    case Status::ARCA: return 0.01;
    case Status::VISA: return 0.005;
    case Status::PREMIUM: return 0.0;
    default: return 0.0;
    }
}
