config_files(text.cpp)

add_executable(text ${SOURCE_OUT})

set(RESOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/resources)
config_target(text INSTALL_SOURCE_OUT RESOURCES_DIR)