cmake_minimum_required(VERSION 3.20.0)
project(structarnica VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)

    # just in case these are not enabled by default
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Zi /MT /EHsc /Oy- /Ob0)
    endif()

    set(CTEST_MEMORYCHECK_COMMAND "drmemory.exe")
    set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "-batch -no_callstack_use_fp -no_callstack_use_top_fp")
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)

    set(CTEST_MEMORYCHECK_COMMAND "valgrind")
    set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--leak-check=yes")
endif()

include(CTest)
enable_testing()

# tests go brrr
add_executable(static_array tests/testStaticArray.cpp)
add_executable(ssl tests/testSingleList.cpp)
add_test(NAME testStaticArray COMMAND static_array)
add_test(NAME testSingleList COMMAND ssl)


include_directories(./include)

# kind of example
add_executable(based main.cpp)

install(DIRECTORY "./include/structarnica" DESTINATION ${CMAKE_INSTALL_PREFIX})