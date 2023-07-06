cmake_minimum_required(VERSION 3.14)
project(UnitTests_CustomVector_PJ)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_NAME UnitTests_CustomVector)


# include FetchContent module
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

message(WARNING "tests:CMakeLists- Project source dir: ${PROJECT_SOURCE_DIR}")
message(WARNING "tests:CMakeLists- CMAKE source dir: ${CMAKE_SOURCE_DIR}")
message(WARNING "tests:CMakeLists- CMAKE curr source dir: ${CMAKE_CURRENT_SOURCE_DIR}")



# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

#add_subdirectory(source/Mult)

add_executable(
  ${EXECUTABLE_NAME}
  "${PROJECT_SOURCE_DIR}/UnitTests_CustomVector.cc"
)

target_include_directories(
  ${EXECUTABLE_NAME} PUBLIC
  "${PROJECT_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(
  ${EXECUTABLE_NAME}
  #Mult
  GTest::gtest_main
)



#look for tests in the hello_test executable
include(GoogleTest)
gtest_discover_tests(${EXECUTABLE_NAME})