cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(vectorPJ VERSION 1.0)

# TODO 1: Replace the following code by:
# * Creating an interface library called tutorial_compiler_flags
#   Hint: use add_library() with the INTERFACE signature
# * Add compiler feature cxx_std_11 to tutorial_compiler_flags
#   Hint: Use target_compile_features()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(tests)

enable_testing()

add_executable(CustomVector src/main.cpp)



# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(CustomVector PUBLIC
                           "${PROJECT_SOURCE_DIR}/include"
                           )
