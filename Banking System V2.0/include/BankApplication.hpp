#ifndef BANK_APPLICATION_H
#define BANK_APPLICATION_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include "DatabaseData.hpp"
#include "DatabaseMeta.hpp"
#include "CubeEncoder.hpp"
#include "Login.hpp"
#include "Registration.hpp"
#include "Utility.hpp"
#include "User.hpp"
#include "LogService.hpp"

class BankApplication {
public:
    BankApplication();
    ~BankApplication();

    void run(); 

private:
    DatabaseData data;
    DatabaseMeta meta;
    CubeEncoder encode; 
    LogService log;

    void showWelcomeMenu();
    void showRegistrationMenu();
    void registerMenu();
    void loginMenu();
};

#endif
