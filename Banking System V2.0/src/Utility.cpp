#include "../include/Utility.hpp"

void Utility::anyKey(){
    std::string tmp;
    std::cout << "Press any key to continue: ";
    std::cin >> tmp;
}

void Utility::clearScreen() {
    system("clear");
}

int Utility::myatoi(const std::string& tmp){
    int result {0};
    for(auto& i : tmp){
        if(i >= '0' && i <= '9') result = result * 10 + (i - '0');
    }
    return result;
}

std::string Utility::currentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&time_now);
    std::stringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
