cmake_minimum_required(VERSION 3.10) 
project(app) 
set(CMAKE_CXX_STANDARD 20) 

add_subdirectory(common) 

add_executable(app main.cpp) 

target_link_libraries(app PRIVATE common)