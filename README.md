# Custom-Vector
This project implements a custom vector container that replicates the functionality of the std::vector class.

## Project Directory Tree
 * [include](./include)
   * [CustomVector.h](./include/CustomVector.h)
 * [src](./src)
   * [main.cpp](./src/main.cpp)
 * [tests](./tests)
   * [CMakeLists.txt](./tests/CMakeLists.txt)
   * [UnitTests_CustomVector.cc](./tests/UnitTests_CustomVector.cc)
 * [CMakeLists.txt](./CMakeLists.txt)
 * [README.md](./README.md)

## Build Instructions (From Linux Terminal)
Requirements: CMake

1. Clone the project

      `git clone https://github.com/lesari08/Custom-Vector.git`

2. Open the cloned projects directory

    `cd Custom-Vector/`  
3. Create a build directory:  
    `mkdir build`  
4. CD to the build directory:  
    `cd build`

5. Generate the build files using CMake:  
    `cmake ..`

6. Build the project:  
    `cmake --build .`

7. Run the executable:  
    `bin/CustomVector`  

Note: Make sure you have CMake installed on your system before proceeding with the build instructions.
