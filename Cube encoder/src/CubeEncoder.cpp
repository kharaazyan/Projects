#include "../include/CubeEncoder.hpp"

CubeEncoder::CubeEncoder(const std::string& text) {
    size = find_size(text.size());
    fill_cube(text);
}

CubeEncoder& CubeEncoder::operator=(const std::string& other) {
    size = find_size(other.size());
    fill_cube(other);  
    return *this;  
}

int CubeEncoder::find_size(int size) const {
    int n {0};
    while (n * n < size){
        ++n;
    }
    return n;
}

void CubeEncoder::fill_cube(const std::string& original_text){
    cube.resize(size, std::vector<char>(size, ' '));
    for(int i = 0; i < original_text.size(); ++i){
        cube[i / size][i % size] = original_text[i];
    }
}

std::string CubeEncoder::right_encode() {
    for (int i = 0; i < size; ++i) {
        char last = cube[i][size - 1];
        for (int j = size - 1; j > 0; --j) {
            cube[i][j] = cube[i][j - 1];
        }
        cube[i][0] = last;
    }
    return final_string();
}

std::string CubeEncoder::left_encode(){
    for (int i = 0; i < size; ++i) {
        char first = cube[i][0];
        for (int j = 0; j < size - 1; ++j) {
            cube[i][j] = cube[i][j + 1];
        }
        cube[i][size - 1] = first;
    }
    return final_string();
}

std::string CubeEncoder::up_encode(){
    std::vector<char> first_row = cube[0];
    for (int i = 0; i < size - 1; ++i) {
        cube[i] = cube[i + 1];
    }
    cube[size - 1] = first_row;
    return final_string();
}

std::string CubeEncoder::down_encode(){
    std::vector<char> last_row = cube[size - 1];
    for (int i = size - 1; i > 0; --i) {
        cube[i] = cube[i - 1];
    }
    cube[0] = last_row;
    return final_string();
}

std::string CubeEncoder::x_encode() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i + j == size - 1 && i < j) {
                std::swap(cube[i][j], cube[j][i]);
            }
        }
    }
    return final_string();
}

std::string CubeEncoder::right_decode(){ return left_encode(); }

std::string CubeEncoder::left_decode() { return right_encode(); }

std::string CubeEncoder::up_decode(){ return down_encode(); }

std::string CubeEncoder::down_decode(){ return up_encode(); }

std::string CubeEncoder::final_string(){
    std::string result;
    for (const auto& row : cube) {
        for (char ch : row) {
            result += ch;
        }
    }
    return result;
}

std::string CubeEncoder::encode(Coder type){
    switch(type){
        case Coder::right: return right_encode(); 
        case Coder::left: return left_encode(); 
        case Coder::up: return up_encode(); 
        case Coder::down: return down_encode(); 
        case Coder::x: return x_encode(); 
        default:
            cout << "Wrong encoding choice! Possible options:(prefix: CubeEncoder::Coder::)\n"
                 << "1) right\n2) left\n3) up\n4) down\n5) x" << endl;
            exit(1);
    }
}

std::string CubeEncoder::decode(Coder type){
    switch(type){
        case Coder::right: return right_decode(); 
        case Coder::left: return left_decode(); 
        case Coder::up: return up_decode(); 
        case Coder::down: return down_decode(); 
        case Coder::x: return x_encode(); 
        default:
            cout << "Wrong decoding choice! Possible options:(prefix: CubeEncoder::Coder::)\n"
                 << "1) right\n2) left\n3) up\n4) down\n5) x" << endl;
            exit(1);
    }
}
