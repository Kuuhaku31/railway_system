project(imgui_setup)

# 设置lib的输出路径为当前文件夹/bini
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

message(STATUS "lib path: ${PROJECT_SOURCE_DIR}")

set(IMGUI_DIR D:/Project/GitHub/imgui)
set(IMGUI_BACKENDS_DIR D:/Project/GitHub/imgui/backends)

aux_source_directory(${IMGUI_DIR} SOURCEFILES)
set(BACKENDS_SOURCEFILES
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_sdlrenderer2.cpp
)

add_library(${PROJECT_NAME} ${SOURCEFILES} ${BACKENDS_SOURCEFILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_DIR} ${IMGUI_BACKENDS_DIR})

# 添加SDL2库、SDL_image库、SDL2_gfx库、SDL2_ttf库、SDL2_mixer库
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL_image REQUIRED SDL2_image)
pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

if(SDL2_FOUND)
    message(STATUS "==SDL2 FOUND==")
    message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
    message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")

    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()

if(SDL_image_FOUND)
    message(STATUS "==SDL_image FOUND==")
    message(STATUS "SDL_image_INCLUDE_DIRS: ${SDL_image_INCLUDE_DIRS}")
    message(STATUS "SDL_image_LIBRARIES: ${SDL_image_LIBRARIES}")

    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_image_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL_image_LIBRARIES})
else()
    message(FATAL_ERROR "SDL_image not found!")
endif()

if(SDL2_GFX_FOUND)
    message(STATUS "==SDL2_GFX FOUND==")
    message(STATUS "SDL2_GFX_INCLUDE_DIRS: ${SDL2_GFX_INCLUDE_DIRS}")
    message(STATUS "SDL2_GFX_LIBRARIES: ${SDL2_GFX_LIBRARIES}")

    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_GFX_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_GFX_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2_gfx not found!")
endif()

if(SDL2_TTF_FOUND)
    message(STATUS "==SDL2_TTF FOUND==")
    message(STATUS "SDL2_TTF_INCLUDE_DIRS: ${SDL2_TTF_INCLUDE_DIRS}")
    message(STATUS "SDL2_TTF_LIBRARIES: ${SDL2_TTF_LIBRARIES}")

    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2_ttf not found!")
endif()

if(SDL2_MIXER_FOUND)
    message(STATUS "==SDL2_MIXER FOUND==")
    message(STATUS "SDL2_MIXER_INCLUDE_DIRS: ${SDL2_MIXER_INCLUDE_DIRS}")
    message(STATUS "SDL2_MIXER_LIBRARIES: ${SDL2_MIXER_LIBRARIES}")

    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_MIXER_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_MIXER_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2_mixer not found!")
endif()

# 设置include目录
set(IMGUISETUP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IMGUI_BACKENDS_DIR}

    ${SDL2_INCLUDE_DIRS}
    ${SDL_image_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}

    PARENT_SCOPE
)
