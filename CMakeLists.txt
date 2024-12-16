cmake_minimum_required(VERSION 3.20)
project(KH_Game_Editor)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# 启用测试
enable_testing()

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(lib)
add_subdirectory(src)
