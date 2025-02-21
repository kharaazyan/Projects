#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <array>
#include "constants.hpp"

struct Registers {
    int r0{0}, r1{0}, r2{0}, r3{0}, r4{0}, r5{0}, r6{0}, wsr{0}, ip{0};
    std::array<char, RAM_S> ram{{'\0'}};
};

#endif // REGISTERS_HPP
