#include "../include/cpu.hpp"
#include <iostream>
#include <string>

int main() {
    CPU cpu;
    std::string command;

    std::cout << "Type START to begin simulation or EXIT to quit:\n";
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, command)) break;
        if (command == "START") {
            cpu.run();
        } else if (command == "EXIT") {
            std::cout << "Terminating CPU simulator...\n";
            break;
        } else {
            std::cout << "Wrong command(START | EXIT)\n";
        }
    }

    return 0;
}
