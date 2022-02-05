cmake_minimum_required(VERSION 3.21)
project(secure_erase_app VERSION 0.1.0.1)

set(CMAKE_CXX_STANDARD 17)

add_executable(secure_erase_app main.cpp getDirectoryInformation.cpp getDirectoryInformation.h shred.cpp)
