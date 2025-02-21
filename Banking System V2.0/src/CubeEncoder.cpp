#include "../include/CubeEncoder.hpp"

CubeEncoder::CubeEncoder(const std::string& text){
    size = find_size(text.size());
    fill_cube(text);
}

CubeEncoder& CubeEncoder::operator=(const std::string& other){
    size = find_size(other.size());
    fill_cube(other);  
    return *this;
}

std::string CubeEncoder::encode(int &code){
    if(code > 99999){
        std::cerr << "Wrong coding type!" << std::endl;
        exit(1);
    }
    code = randInt();
    int tmp {0};
    while(code != 0){
        switch(static_cast<Coder>(code % 10)){
            case Coder::right: right_encode(); break;
            case Coder::left:  left_encode();  break;
            case Coder::up:    up_encode();    break;
            case Coder::down:  down_encode();  break;
            case Coder::x:     x_encode();     break;
            default:           break;
        }
        tmp = tmp * 10 + code % 10;
        code /= 10;
    }
    code = tmp;
    return final_string();
}

std::string CubeEncoder::decode(int &code){
    if(code > 99999){
        std::cerr << "Wrong decoding type!" << std::endl;
        exit(1);
    }
    int tmp {0};
    while(code != 0){
        switch(static_cast<Coder>(code % 10)){
            case Coder::right: right_decode(); break;
            case Coder::left:  left_decode();  break;
            case Coder::up:    up_decode();    break;
            case Coder::down:  down_decode();  break;
            case Coder::x:     x_encode();     break;
            default:           break;
        }
        tmp = tmp * 10 + code % 10;
        code /= 10;
    }
    code = tmp;
    return final_string();
}

int CubeEncoder::randInt(){
    int result {0};
    while(result < 10000){
        result = result * 10 + (1 + (::rand() % 5));
    }
    return result;
}

int CubeEncoder::find_size(int s) const{
    int n {0};
    while (n * n < s){
        ++n;
    }
    return n;
}

void CubeEncoder::fill_cube(const std::string& original_text){
    cube.clear();
    cube.resize(size, std::vector<char>(size, ' '));
    for(int i = 0; i < static_cast<int>(original_text.size()); ++i){
        cube[i / size][i % size] = original_text[i];
    }
}

void CubeEncoder::right_encode(){
    for (int i = 0; i < size; ++i){
        char last = cube[i][size - 1];
        for (int j = size - 1; j > 0; --j){
            cube[i][j] = cube[i][j - 1];
        }
        cube[i][0] = last;
    }
}

void CubeEncoder::left_encode(){
    for (int i = 0; i < size; ++i) {
        char first = cube[i][0];
        for (int j = 0; j < size - 1; ++j) {
            cube[i][j] = cube[i][j + 1];
        }
        cube[i][size - 1] = first;
    }
}

void CubeEncoder::up_encode(){
    std::vector<char> first_row = cube[0];
    for (int i = 0; i < size - 1; ++i) {
        cube[i] = cube[i + 1];
    }
    cube[size - 1] = first_row;
}

void CubeEncoder::down_encode(){
    std::vector<char> last_row = cube[size - 1];
    for (int i = size - 1; i > 0; --i) {
        cube[i] = cube[i - 1];
    }
    cube[0] = last_row;
}

void CubeEncoder::x_encode() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i + j == size - 1 && i < j) {
                std::swap(cube[i][j], cube[j][i]);
            }
        }
    }
}

std::string CubeEncoder::final_string(){
    std::string result;
    for (const auto& row : cube) {
        for (char ch : row) {
            result += ch;
        }
    }
    return result;
}
