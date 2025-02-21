#include "../include/BankApplication.hpp"

BankApplication::BankApplication()
    : data(), meta(), encode("") 
{
}

BankApplication::~BankApplication() {}

void BankApplication::run() {
    std::string choice;
    while (true) {
        Utility::clearScreen();
        showWelcomeMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == "1") {
            registerMenu();
        } else if (choice == "2") {
            loginMenu();
        } else if (choice == "3") {
            std::cout << "Thank you for using Kharazyan Bank!" << std::endl;
            return;
        } else {
            std::cout << "Oops! That's not a valid choice." << std::endl;
            Utility::anyKey();
        }
    }
}

void BankApplication::showWelcomeMenu() {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "        Welcome to Kharazyan Bank!" << std::endl;
    std::cout << "Your gateway to stellar financial services." << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "1. Register" << std::endl;
    std::cout << "2. Log in" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Please choose an option: ";
}

void BankApplication::showRegistrationMenu() {
    Utility::clearScreen();
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "    Registration Menu              " << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "1. Register Arca User" << std::endl;
    std::cout << "2. Register Visa User" << std::endl;
    std::cout << "3. Register Premium User" << std::endl;
    std::cout << "4. Back to Main Menu" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Please choose an option: ";
}

void BankApplication::registerMenu() {
   std::string regChoice;
   std::unique_ptr<RegistrationFactory> factory;

   while (true) {
      showRegistrationMenu();
      std::cin >> regChoice;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      if (regChoice == "1") {
          std::cout << "Registering Arca User..." << std::endl;
          factory = std::make_unique<RegArcaFactory>(data, meta, encode, log);
          factory->createRegistration(); 
      }
      else if (regChoice == "2") {
          std::cout << "Registering Visa User..." << std::endl;
          factory = std::make_unique<RegVisaFactory>(data, meta, encode, log);
          factory->createRegistration(); 
      }
      else if (regChoice == "3") {
          std::cout << "Registering Premium User..." << std::endl;
          factory = std::make_unique<RegPremiumFactory>(data, meta, encode, log);
          factory->createRegistration(); 
      }
      else if (regChoice == "4") {
          std::cout << "Returning to main menu..." << std::endl;
          break;
      }
      else {
          std::cout << "Oops! That's not a valid choice." << std::endl;
      }
   }
}

void BankApplication::loginMenu() {
    Utility::clearScreen();
    std::unique_ptr<AccountMembers> tmpMember;
    {
        std::unique_ptr<Login> ptr = std::make_unique<Login>(data, meta, encode, log);
        if (!ptr->doLogin()) return;
        else tmpMember = ptr->get();
    } 
    User user(std::move(tmpMember), data, meta, encode, log);
    user.showMainMenu();
}
