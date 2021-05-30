config_files(custom_shader.cpp)

add_executable(custom_shader ${SOURCE_OUT})

set(RESSOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/ressources)
config_target(custom_shader INSTALL_SOURCE_OUT RESSOURCES_DIR)