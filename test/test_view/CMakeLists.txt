# tests

project(test_view)

# 添加include目录
include_directories(../../src/view)
include_directories(../../src/entity)
include_directories(../../src/controller)
include_directories(./train_controller_test)

# 添加源文件
aux_source_directory(./ DIR_SRCS)
aux_source_directory(../../lib/imgui_setup DIR_SRCS)
aux_source_directory(../../src/view DIR_SRCS)
aux_source_directory(../../src/entity DIR_SRCS)
aux_source_directory(../../src/controller DIR_SRCS)
aux_source_directory(./train_controller_test DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

# 添加imgui_setup库
include_directories(${IMGUISETUP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} imgui_setup)

# test_table
project(test_table)
aux_source_directory(./test_table DIR_SRCS_TABLE)
aux_source_directory(../lib DIR_SRCS_TABLE)

#
# project(test_model CXX)

# set(MODEL_SRC
# ../src/model/train_model.c
# ../src/model/train_model.h
# ../src/model/train_model.c
# ../src/model/train_model.h
# )

# set(ENTITY_SRC
# ../src/entity/train.h
# ../src/entity/order.h
# )

# set(UTILS_SRC
# ../src/utils/error.h
# ../src/utils/utils.h
# ../src/utils/utils.c
# )
# file(COPY ../lib/sqlite3/sqlite3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# add_executable(test_model ${MODEL_SRC} ${ENTITY_SRC} ${UTILS_SRC}
# test_model/test_train_model.cpp)
# set(HEADER_DIR ../src ../src/view ../src/model ../src/entity ../src/controller ../src/utils)
# target_link_directories(test_model PUBLIC ../lib/sqlite3)
# target_include_directories(test_model PUBLIC
# ../lib/sqlite3 ../lib/imgui
# ${HEADER_DIR}
# )
# target_link_libraries(test_model PUBLIC sqlite3 "-lstdc++exp")