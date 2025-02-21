#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

bool isRegister(const std::string &r);
bool isNumber(const std::string &val);
std::string trim(const std::string &str);

// Парсинг команд
bool parseTwoOperands(const std::string &cmd, std::string &dest, std::string &src);
bool parseThreeOperands(const std::string &cmd, std::string &rd, std::string &rs, std::string &val);

#endif // UTILS_HPP
