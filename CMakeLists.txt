project(compass CXX)
cmake_minimum_required(VERSION 2.8)

add_executable( compass main.cpp i2c.h i2c.cpp compass.h compass.cpp )
