cmake_minimum_required(VERSION 3.20)

project(imgui)

aux_source_directory(. SRC)

add_library(imgui STATIC ${SRC}
        backends/imgui_impl_sdlrenderer2.cpp
        backends/imgui_impl_sdl2.cpp
        backends/imgui_impl_sdl2.h
        backends/imgui_impl_sdlrenderer2.h)

target_include_directories(imgui PUBLIC . ../sdl2/include/SDL2)

target_link_directories(imgui PUBLIC ../sdl2/lib)
target_link_libraries(imgui PUBLIC SDL2)
