cmake_minimum_required(VERSION 3.24)
project(LABSYS4)

set(CMAKE_CXX_STANDARD 17)

add_executable(LABSYS4 server.cpp client.cpp common.h)
