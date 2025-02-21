#include "../include/utils.hpp"
#include <cctype>
#include <algorithm>
#include <sstream>

bool isRegister(const std::string &r) {
    return (r.size() == 2 && r[0] == 'R' && (r[1] >= '0' && r[1] <= '6'));
}

bool isNumber(const std::string &val) {
    if (val.empty()) return false;
    for (auto c : val) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

std::string trim(const std::string &str) {
    std::string s = str;
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
    return s;
}

bool parseTwoOperands(const std::string &cmd, std::string &dest, std::string &src) {
    std::istringstream iss(cmd);
    std::string opcode; 
    iss >> opcode;
    std::string line;
    std::getline(iss, line);
    line = trim(line);

    size_t commaPos = line.find(',');
    if (commaPos == std::string::npos) return false;

    std::string op1 = trim(line.substr(0, commaPos));
    std::string op2 = trim(line.substr(commaPos + 1));

    if (op1.empty() || op2.empty()) return false;

    dest = op1;
    src = op2;
    return true;
}

bool parseThreeOperands(const std::string &cmd, std::string &rd, std::string &rs, std::string &val) {
    std::istringstream iss(cmd);
    std::string opcode; 
    iss >> opcode;
    std::string line;
    std::getline(iss, line);
    line = trim(line);

    size_t firstComma = line.find(',');
    if (firstComma == std::string::npos) return false;

    std::string firstPart = trim(line.substr(0, firstComma));
    std::string rest = trim(line.substr(firstComma + 1));

    size_t secondComma = rest.find(',');
    if (secondComma == std::string::npos) return false;

    std::string secondPart = trim(rest.substr(0, secondComma));
    std::string thirdPart = trim(rest.substr(secondComma + 1));

    if (firstPart.empty() || secondPart.empty() || thirdPart.empty()) return false;

    rd = firstPart;
    rs = secondPart;
    val = thirdPart;
    return true;
}
