#ifndef CPU_HPP
#define CPU_HPP

#include <fstream>
#include <string>
#include "registers.hpp"
#include "instructions.hpp"

class CPU {
public:
    CPU();
    ~CPU();
    void run();

private:
    Registers reg;
    std::fstream file;

    void executionLoop();

    void addCmd(const std::string &cmd);
    void subCmd(const std::string &cmd);
    void movCmd(const std::string &cmd);
    void loadCmd(const std::string &cmd);
    void storeCmd(const std::string &cmd);
    void discCmd(const std::string &cmd);
    void layoCmd();
    void exitCmd();

    void reg_init();
    void saveState();
    int  get_value(const std::string &regName) const;
    void set_value(const std::string &regName, int value);
    Instruction analyze_command(const std::string &cmd);
    bool stugum() const;
};

#endif // CPU_HPP
