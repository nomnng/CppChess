cmake_minimum_required(VERSION 3.10)
project(CppChess CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(WINAPI_BUILD "Build WinApi version" ON)

file(GLOB SOURCES "src/*.cpp")
if (WINAPI_BUILD)
    list(FILTER SOURCES EXCLUDE REGEX "game_console.cpp")
    add_compile_definitions(WINAPI_BUILD)
else()
    list(FILTER SOURCES EXCLUDE REGEX "game_winapi.cpp")
    add_compile_definitions(CONSOLE_BUILD)
endif()

add_executable(cpp_chess ${SOURCES})

add_compile_definitions(UNICODE _UNICODE)
