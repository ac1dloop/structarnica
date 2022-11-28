cmake_minimum_required(VERSION 3.20.0)
project(structarnica VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

include(CTest)
enable_testing()

# tests go brrr
add_executable(static_array tests/testStaticArray.cpp)
add_test(NAME testStaticArray COMMAND static_array)

include_directories(./include)

# kind of example
add_executable(based main.cpp)

install(DIRECTORY "./include/structarnica" DESTINATION ${CMAKE_INSTALL_PREFIX})