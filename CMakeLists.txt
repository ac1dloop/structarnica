cmake_minimum_required(VERSION 3.20.0)
project(structarnica VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include_directories(./include/structarnica)

# kind of example
add_executable(based main.cpp)

install(DIRECTORY "./include/structarnica" DESTINATION ${CMAKE_INSTALL_PREFIX})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
