#include "../include/LogService.hpp"

LogService::LogService(const std::string &filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

LogService::~LogService() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void LogService::log(const std::string &message) {
    if (logFile.is_open()) {
        logFile << "[" << Utility::currentTime() << "] " << message << std::endl;
    } else {
        std::cerr << "Log file not open!" << std::endl;
        exit(1);
    }
    logFile.flush();
}

void LogService::showLog(std::string &login) const {
    std::ifstream ifs("log.txt");
    if (!ifs.is_open()) {
        std::cerr << "Cannot open log file for reading." << std::endl;
        return;
    }

    std::string line;
    std::string pattern = "User [" + login + "]";

    while (std::getline(ifs, line)) {
        if (line.find(pattern) != std::string::npos) {
            std::cout << line << std::endl;
        }
    }
    ifs.close();
}


void LogService::showLog() const {
    std::ifstream ifs("log.txt");
    if (!ifs.is_open()) {
        std::cerr << "Cannot open log file for reading." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(ifs, line)) {
        std::cout << line << std::endl;
    }
    ifs.close();
}
