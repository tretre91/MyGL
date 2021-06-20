set(SOURCE_IN ${CMAKE_CURRENT_LIST_DIR}/simple_program.cpp)

add_executable(simple_program ${SOURCE_IN})

config_target(simple_program SOURCE_IN "")