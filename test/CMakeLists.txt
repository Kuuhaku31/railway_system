# tests

project(test_view)

# 添加include目录
include_directories(../src/view)
include_directories(../src/entity)

# 添加源文件
aux_source_directory(./test_view DIR_SRCS)
aux_source_directory(../src/view DIR_SRCS)
aux_source_directory(../src/entity DIR_SRCS)

aux_source_directory(../lib DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

# 添加imgui_setup库
include_directories(${IMGUISETUP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} imgui_setup)
