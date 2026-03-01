cmake_minimum_required(VERSION 3.10)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

add_subdirectory(JsonReader)
add_subdirectory(SerialLibrary)

add_subdirectory(common)

add_executable(app main.cpp)

target_link_libraries(app PRIVATE 
    common_lib
    json_lib
    serial
    Threads::Threads
    rt
)
