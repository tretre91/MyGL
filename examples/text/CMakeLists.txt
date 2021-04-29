config_files(text.cpp)

add_executable(text ${SOURCE_OUT})

set(RESSOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/ressources)
config_target(text INSTALL_SOURCE_OUT RESSOURCES_DIR)