config_files(magnifying_glass.cpp)

add_executable(magnifying_glass ${SOURCE_OUT})

set(RESSOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/ressources)
config_target(magnifying_glass INSTALL_SOURCE_OUT RESSOURCES_DIR)