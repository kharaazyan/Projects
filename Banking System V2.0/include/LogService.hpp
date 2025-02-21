#ifndef LOGSERVICE_HPP
#define LOGSERVICE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "Utility.hpp"

class LogService {
private:
    std::ofstream logFile;

public:
    LogService(const std::string &filename = "log.txt");
    ~LogService();
    void log(const std::string &message);
    void showLog(std::string &login) const;
    void showLog() const;
};

#endif // LOGSERVICE_HPP
