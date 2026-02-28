cmake_minimum_required(VERSION 3.10)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

if(NOT JSONCPP_FOUND)
    message(STATUS "System jsoncpp not found, using subdirectory")
    add_subdirectory(JsonReader/jsoncpp)
    set(JSON_LIB jsoncpp_lib)
else()
    message(STATUS "Using system jsoncpp")
    set(JSON_LIB ${JSONCPP_LIBRARIES})
    include_directories(${JSONCPP_INCLUDE_DIRS})
endif()

add_subdirectory(SerialLibrary/serial)
add_subdirectory(common)

add_executable(app main.cpp)

target_link_libraries(app PRIVATE 
    common_lib
    ${JSON_LIB}
    serial
    Threads::Threads
    rt
)
