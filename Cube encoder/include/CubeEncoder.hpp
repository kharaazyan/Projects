#ifndef CUBEENCODER_HPP
#define CUBEENCODER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

class CubeEncoder {
public:
    enum class Coder {
        none, right, left, up, down, x
    };

    CubeEncoder(const std::string& text);

    CubeEncoder& operator=(const std::string& other);

    std::string encode(Coder type = Coder::none);
    std::string decode(Coder type = Coder::none);

private:
    std::vector<std::vector<char>> cube;
    int size {0};

    int find_size(int size) const;
    void fill_cube(const std::string& original_text);
    std::string right_encode();
    std::string left_encode();
    std::string up_encode();
    std::string down_encode();
    std::string x_encode();
    std::string right_decode();
    std::string left_decode();
    std::string up_decode();
    std::string down_decode();
    std::string final_string();
};

#endif // CUBEENCODER_HPP
