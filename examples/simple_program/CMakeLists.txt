set(SOURCE ${CMAKE_CURRENT_LIST_DIR}/simple_program.cpp)

add_executable(simple_program ${SOURCE})

config_target(simple_program FILES ${SOURCE})
