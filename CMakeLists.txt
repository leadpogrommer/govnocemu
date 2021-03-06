cmake_minimum_required(VERSION 3.19)
project(govnocemu C)

set(CMAKE_C_STANDARD 11)

add_definitions(-DDEBUG_PRINT)
add_executable(govnocemu main.c memory.c memory.h cpu.h cpu.c)