#include "include/CubeEncoder.hpp"

int main(){
    CubeEncoder encoder("Hovhannes Kharazyan");
    
    std::string encoded = encoder.encode(CubeEncoder::Coder::right);
    encoded = encoder.encode(CubeEncoder::Coder::x);    
    std::cout << "Encoded: " << encoded << std::endl;
    
    encoder = encoded;
    encoded = encoder.decode(CubeEncoder::Coder::x);
    encoded = encoder.decode(CubeEncoder::Coder::right);
    std::cout << "Decoded: " << encoded << std::endl;
    
    return 0;
}
