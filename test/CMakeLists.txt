# tests
add_subdirectory(test_view)
# test_table
project(test_table)
aux_source_directory(./test_table DIR_SRCS_TABLE)
aux_source_directory(../lib DIR_SRCS_TABLE)

project(test_controller CXX)

set(SRC
        ../src/controller/train_controller.c
        ../src/controller/train_controller.h
)
set(MODEL_SRC
        ../src/model/train_model.c
        ../src/model/train_model.h
        ../src/model/train_model.c
        ../src/model/train_model.h
)
set(ENTITY_SRC
        ../src/entity/train.h
        ../src/entity/order.h
)

set(UTILS_SRC
        ../src/utils/error.h
        ../src/utils/utils.h
        ../src/utils/utils.c
)
file(COPY ../lib/sqlite3/sqlite3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_executable(test_controller ${MODEL_SRC} ${SRC} ${ENTITY_SRC} ${UTILS_SRC}
        test_controller/test_controller.cpp)
set(HEADER_DIR ../src ../src/view ../src/model ../src/entity ../src/controller ../src/utils)
target_link_directories(test_controller PUBLIC ../lib/sqlite3)
target_include_directories(test_controller PUBLIC
        ../lib/sqlite3 ../lib/imgui
        ${HEADER_DIR}
)
target_link_libraries(test_controller PUBLIC sqlite3 "-lstdc++exp")

 project(test_model CXX)

 set(MODEL_SRC
 ../src/model/train_model.c
 ../src/model/train_model.h
 ../src/model/train_model.c
 ../src/model/train_model.h
 )

 set(ENTITY_SRC
 ../src/entity/train.h
 ../src/entity/order.h
 )

 set(UTILS_SRC
 ../src/utils/error.h
 ../src/utils/utils.h
 ../src/utils/utils.c
 )
 file(COPY ../lib/sqlite3/sqlite3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
 add_executable(test_model ${MODEL_SRC} ${ENTITY_SRC} ${UTILS_SRC}
 test_model/test_train_model.cpp)
 set(HEADER_DIR ../src ../src/view ../src/model ../src/entity ../src/controller ../src/utils)
 target_link_directories(test_model PUBLIC ../lib/sqlite3)
 target_include_directories(test_model PUBLIC
 ../lib/sqlite3 ../lib/imgui
 ${HEADER_DIR}
 )
 target_link_libraries(test_model PUBLIC sqlite3 "-lstdc++exp")