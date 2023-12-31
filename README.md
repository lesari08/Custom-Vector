# Custom-Vector
This project implements a custom vector container that replicates the functionality of the std::vector class.

## Project Directory Tree
 * [include](./include)
   * [CustomVector.h](./include/CustomVector.h)
 * [src](./src)
   * [main.cpp](./src/main.cpp)
 * [tests](./tests)
   * [CMakeLists.txt](./tests/CMakeLists.txt)
   * [UnitTests_CustomVector.cpp](./tests/UnitTests_CustomVector.cpp)
 * [CMakeLists.txt](./CMakeLists.txt)
 * [README.md](./README.md)


## Implemented Methods
All Member Functions of [std::vector](https://en.cppreference.com/w/cpp/container/vector) (as of C++20)

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

## Manual Testing 
To run the suite of functional unit tests(using Google Test Framework), follow the prior build instruction steps 1 through 6.
Finally, run the executable named bin/UnitTests_CustomVector. The results will be found under directory Custom-Vector/build/unit_test_results

## Automated Testing with Jenkins
This repository is configured with automated server Jenkins, so after each commit to this repository, functional unit tests are automatically run, as well as Valgrind Memcheck to test for any memory-related issues.
