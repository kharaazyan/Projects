#ifndef UTILITY_H
#define UTILITY_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>



class Utility {
public:
    static void anyKey();    
    static void clearScreen();
    static int myatoi(const std::string& tmp);
    static std::string currentTime();
};

#endif
