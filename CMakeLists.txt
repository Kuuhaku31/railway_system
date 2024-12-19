cmake_minimum_required(VERSION 3.20)
project(RailwaySystem)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# 启用测试
enable_testing()

# 设置生成的可执行文件的输出路径为bin目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
