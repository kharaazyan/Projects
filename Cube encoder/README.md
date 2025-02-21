**CubeEncoder**

CubeEncoder is a C++ library that provides encoding and decoding functionalities using a cube-based cipher. It allows various transformations such as shifting rows, shifting columns, and swapping elements diagonally.

**Table of Contents**

- Features  
- Project Structure  
- Requirements  
- Installation  
- Usage  
- Example  
- Build Instructions  

**Features**

- Right Encoding: Shifts each row to the right.  
- Left Encoding: Shifts each row to the left.  
- Up Encoding: Shifts the entire cube upwards.  
- Down Encoding: Shifts the entire cube downwards.  
- X Encoding: Swaps elements along the diagonal.

**Project Structure**

```
CubeEncoder/
    include/
        CubeEncoder.hpp
    src/
        CubeEncoder.cpp
    main.cpp
    README.md
```

**Requirements**

- C++11 or higher  
- C++ Compiler (e.g., g++, clang++)

**Installation**

1. Clone the repository:  
   `git clone https://github.com/kharaazyan/CubeEncoder.git`  
   `cd CubeEncoder`

2. Create a build directory:  
   `mkdir build`  
   `cd build`

3. Compile the project:  
   `g++ -std=c++11 ../src/CubeEncoder.cpp ../main.cpp -I../include -o CubeEncoder`

4. Run the executable:  
   `./CubeEncoder`

**Usage**

1. Include the header:  
   ```cpp
   #include "CubeEncoder.hpp"
   ```

2. Create an instance:  
   ```cpp
   CubeEncoder encoder("Your text here");
   ```

3. Encode:  
   ```cpp
   std::string encoded = encoder.encode(CubeEncoder::Coder::right);
   ```

4. Decode:  
   ```cpp
   std::string decoded = encoder.decode(CubeEncoder::Coder::right);
   ```

**Example**

```cpp
#include <iostream>
#include "CubeEncoder.hpp"

int main() {
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
```

**Build Instructions**

1. Navigate to the project directory:  
   `cd CubeEncoder`

2. Create and navigate to the build directory:  
   `mkdir build`  
   `cd build`

3. Compile the project:  
   `g++ -std=c++11 ../src/CubeEncoder.cpp ../main.cpp -I../include -o CubeEncoder`

4. Run the executable:  
   `./CubeEncoder`