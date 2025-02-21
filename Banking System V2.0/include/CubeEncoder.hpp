#ifndef CUBE_ENCODER_H
#define CUBE_ENCODER_H

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

class CubeEncoder {
private:
    std::vector<std::vector<char>> cube;
    int size {0};

    int randInt();   
    int find_size(int size) const;
    void fill_cube(const std::string& original_text);

    void right_encode();
    void left_encode();
    void up_encode();
    void down_encode();
    void x_encode();

    std::string final_string();

    inline void right_decode(){ left_encode(); }
    inline void left_decode() { right_encode(); }
    inline void up_decode(){ down_encode(); }
    inline void down_decode(){ up_encode(); }

public:
    enum class Coder {
        none, right, left, up, down, x
    };

    CubeEncoder(const std::string& text);
    CubeEncoder& operator=(const std::string& other);

    std::string encode(int &code);
    std::string decode(int &code);
};

#endif
