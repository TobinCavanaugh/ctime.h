cmake_minimum_required(VERSION 3.25)
project(ctime C)

set(CMAKE_C_STANDARD 90)

add_executable(ctime ctime.h main.c)

set_target_properties(ctime PROPERTIES C_STANDARD 90)