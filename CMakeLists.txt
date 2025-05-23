cmake_minimum_required(VERSION 3.16)
project(Cat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64" ${CMAKE_PREFIX_PATH})

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt6_standard_project_setup()

file(GLOB_RECURSE SOURCES 
    "src/*.cpp" 
    "src/*.h"
    "include/*.h"
)

add_executable(cat ${SOURCES})

target_include_directories(Cat PRIVATE include)
target_link_libraries(Cat Qt6::Core Qt6::Widgets)