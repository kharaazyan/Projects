#include "../include/cpu.hpp"
#include "../include/utils.hpp"
#include "constants.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <unistd.h> 

CPU::CPU() {
    reg_init();
}

CPU::~CPU() {
    if (file.is_open()) file.close();
    std::remove(MEMORY_FILE.c_str());
}

void CPU::run() {
    system("clear");
    reg_init();
    file.open(MEMORY_FILE, std::ios::in | std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Error opening memory file." << std::endl;
        return;
    }

    std::cout << "Initial ";
    layoCmd();
    executionLoop();
}

void CPU::executionLoop() {
    reg.wsr = 1;
    std::string input;
    while (reg.wsr) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;
        input = trim(input);

        Instruction inst = analyze_command(input);
        switch (inst) {
            case ADD:   addCmd(input); break;
            case SUB:   subCmd(input); break;
            case MOV:   movCmd(input); break;
            case LOAD:  loadCmd(input); break;
            case STORE: storeCmd(input); break;
            case DISC:  discCmd(input); break;
            case LAYO:  layoCmd(); break;
            case EXIT:  exitCmd(); return;
            default:
                std::cout << "Unknown or invalid command.\n";
                break;
        }
    }
}

Instruction CPU::analyze_command(const std::string &cmd) {
    std::istringstream iss(cmd);
    std::string token;
    iss >> token;
    if (token == "ADD")   return ADD;
    if (token == "SUB")   return SUB;
    if (token == "MOV")   return MOV;
    if (token == "LOAD")  return LOAD;
    if (token == "STORE") return STORE;
    if (token == "DISC")  return DISC;
    if (token == "LAYO")  return LAYO;
    if (token == "EXIT")  return EXIT;
    return INVALID;
}

void CPU::reg_init() {
    reg.ip = 0;
    reg.r0 = 0; reg.r1 = 0; reg.r2 = 0; reg.r3 = 0;
    reg.r4 = 0; reg.r5 = 0; reg.r6 = 0;
    reg.wsr = 0;
    reg.ram.fill('\0');
}

bool CPU::stugum() const {
    for (auto c : reg.ram) {
        if (c != ' ' && c != '\0') return true;
    }
    return false;
}

int CPU::get_value(const std::string &regName) const {
    if (regName == "R0") return reg.r0;
    if (regName == "R1") return reg.r1;
    if (regName == "R2") return reg.r2;
    if (regName == "R3") return reg.r3;
    if (regName == "R4") return reg.r4;
    if (regName == "R5") return reg.r5;
    if (regName == "R6") return reg.r6;
    return 0;
}

void CPU::set_value(const std::string &regName, int value) {
    if (regName == "R0") reg.r0 = value;
    else if (regName == "R1") reg.r1 = value;
    else if (regName == "R2") reg.r2 = value;
    else if (regName == "R3") reg.r3 = value;
    else if (regName == "R4") reg.r4 = value;
    else if (regName == "R5") reg.r5 = value;
    else if (regName == "R6") reg.r6 = value;
}

void CPU::saveState() {
    for (auto &c : reg.ram) {
        if (c == '\0') c = ' ';
    }
    reg.ram[RAM_S - 1] = '\0';

    if (!file.is_open()) return;

    file.seekp(0, std::ios::end);
    char buff[BUFF] = {'\0'};
    std::snprintf(buff, BUFF,
        "R0: %d R1: %d R2: %d R3: %d R4: %d R5: %d R6: %d IP: %d WSR: %d\nMemory: %s\n",
        reg.r0, reg.r1, reg.r2, reg.r3, reg.r4, reg.r5, reg.r6, reg.ip, reg.wsr, reg.ram.data());

    file.write(buff, POS_DFL);
    file.flush();
}

void CPU::addCmd(const std::string &cmd) {
    std::string rd, rs, val;
    if (!parseThreeOperands(cmd, rd, rs, val)) {
        std::cout << "Wrong syntax(example: ADD R1, R2, R3)\n";
        return;
    }
    if (!isRegister(rd) || !isRegister(rs)) {
        std::cout << "Wrong register name!\n";
        return;
    }

    int second = get_value(rs);
    int third = 0;
    if (isRegister(val)) {
        third = get_value(val);
    } else if (isNumber(val)) {
        third = std::stoi(val);
    } else {
        std::cout << "Invalid operand for ADD.\n";
        return;
    }

    set_value(rd, second + third);
    reg.ip++;
    saveState();
}

void CPU::subCmd(const std::string &cmd) {
    std::string rd, rs, val;
    if (!parseThreeOperands(cmd, rd, rs, val)) {
        std::cout << "Wrong syntax(example: SUB R1, R2, R3)\n";
        return;
    }
    if (!isRegister(rd) || !isRegister(rs)) {
        std::cout << "Wrong register name!\n";
        return;
    }

    int second = get_value(rs);
    int third = 0;
    if (isRegister(val)) {
        third = get_value(val);
    } else if (isNumber(val)) {
        third = std::stoi(val);
    } else {
        std::cout << "Invalid operand for SUB.\n";
        return;
    }

    set_value(rd, second - third);
    reg.ip++;
    saveState();
}

void CPU::movCmd(const std::string &cmd) {
    std::string rd, val;
    if (!parseTwoOperands(cmd, rd, val)) {
        std::cout << "Wrong syntax(example: MOV R1, R2 or MOV R1, 45)\n";
        return;
    }
    if (!isRegister(rd)) {
        std::cout << "Wrong register name!\n";
        return;
    }

    int result = 0;
    if (isRegister(val)) {
        result = get_value(val);
    } else if (isNumber(val)) {
        result = std::stoi(val);
    } else {
        std::cout << "Invalid operand for MOV.\n";
        return;
    }

    set_value(rd, result);
    reg.ip++;
    saveState();
}

void CPU::loadCmd(const std::string &cmd) {
    std::string rd, idxStr;
    if (!parseTwoOperands(cmd, rd, idxStr)) {
        std::cout << "Wrong syntax(example: LOAD R1, 12)\n";
        return;
    }
    if (!isRegister(rd)) {
        std::cout << "Wrong register name!\n";
        return;
    }
    if (!isNumber(idxStr)) {
        std::cout << "Invalid index for LOAD.\n";
        return;
    }

    int index = std::stoi(idxStr);
    if (index < 0 || index >= RAM_S) {
        std::cout << "Index out of range.\n";
        return;
    }

    reg.ram[index] = static_cast<char>(get_value(rd));
    reg.ip++;
    saveState();
}

void CPU::storeCmd(const std::string &cmd) {
    std::string rd, idxStr;
    if (!parseTwoOperands(cmd, rd, idxStr)) {
        std::cout << "Wrong syntax(example: STORE R1, 12)\n";
        return;
    }
    if (!isRegister(rd)) {
        std::cout << "Wrong register name!\n";
        return;
    }
    if (!isNumber(idxStr)) {
        std::cout << "Invalid index for STORE.\n";
        return;
    }

    int index = std::stoi(idxStr);
    if (index < 0 || index >= RAM_S) {
        std::cout << "Index out of range.\n";
        return;
    }

    int val = static_cast<int>(reg.ram[index]);
    set_value(rd, val);
    reg.ram[index] = 0;
    reg.ip++;
    saveState();
}

void CPU::discCmd(const std::string &cmd) {
    std::istringstream iss(cmd);
    std::string opcode; 
    int idx;
    iss >> opcode >> idx;
    if (iss.fail()) {
        std::cout << "Wrong syntax(example: DISC 2)\n";
        return;
    }

    file.clear();
    file.seekg(-1 * (idx * POS_DFL), std::ios::cur);
    std::streampos size = file.tellg();
    if (size < 0) {
        std::cout << "Cannot DISC: invalid position.\n";
        return;
    }

    file.clear();
    file.seekg(0, std::ios::end);
    std::streampos endPos = file.tellg();
    std::streampos newSize = size;
    if (newSize < 0) newSize = 0;
    file.close();
    truncate(MEMORY_FILE.c_str(), newSize);
    file.open(MEMORY_FILE, std::ios::in | std::ios::out);
    file.seekg(-1 * POS_DFL, std::ios::cur);

    char buff[BUFF];
    file.read(buff, POS_DFL);
    buff[POS_DFL - 1] = '\0';
    int ip, wsr;
    int r0, r1, r2, r3, r4, r5, r6;
    char mem[RAM_S];
    std::sscanf(buff,
        "R0: %d R1: %d R2: %d R3: %d R4: %d R5: %d R6: %d IP: %d WSR: %d\nMemory: %255s",
        &r0, &r1, &r2, &r3, &r4, &r5, &r6, &ip, &wsr, mem);

    reg.r0 = r0; reg.r1 = r1; reg.r2 = r2; reg.r3 = r3; 
    reg.r4 = r4; reg.r5 = r5; reg.r6 = r6; reg.ip = ip; reg.wsr = wsr;
    for (int i = 0; i < RAM_S; i++) {
        reg.ram[i] = mem[i];
        if (mem[i] == '\0') break;
    }
}

void CPU::layoCmd() {
    std::cout << "CPU state:\n"
              << "R0: " << reg.r0 << ", R1: " << reg.r1 << ", R2: " << reg.r2 
              << ", R3: " << reg.r3 << ", R4: " << reg.r4 << ", R5: " << reg.r5 
              << ", R6: " << reg.r6 << ", WSR: " << reg.wsr << ", IP: " << reg.ip << "\n\nMemory: {"
              << (stugum() ? reg.ram.data() : "0...0") << "}\n";
}

void CPU::exitCmd() {
    reg.wsr = 0;
    if (file.is_open()) {
        file.close();
        std::remove(MEMORY_FILE.c_str());
    }
    std::cout << "You have successfully exited the simulation\n";
}
