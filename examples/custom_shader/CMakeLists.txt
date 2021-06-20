config_files(custom_shader.cpp)

add_executable(custom_shader ${SOURCE_OUT})

set(RESOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/resources)
config_target(custom_shader INSTALL_SOURCE_OUT RESOURCES_DIR)