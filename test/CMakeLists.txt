# tests

project(test_view)

# 添加include目录
include_directories(../src/view)
include_directories(../src/entity)
include_directories(../src/controler)

# 添加源文件
aux_source_directory(./test_view DIR_SRCS)
aux_source_directory(../src/view DIR_SRCS)
aux_source_directory(../src/entity DIR_SRCS)
aux_source_directory(../src/controler DIR_SRCS)

aux_source_directory(../lib DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

# 添加imgui_setup库
include_directories(${IMGUISETUP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} imgui_setup)

# test_table
project(test_table)
aux_source_directory(./test_table DIR_SRCS_TABLE)
aux_source_directory(../lib DIR_SRCS_TABLE)

add_executable(${PROJECT_NAME} ${DIR_SRCS_TABLE})

# 添加imgui_setup库
include_directories(${IMGUISETUP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} imgui_setup)