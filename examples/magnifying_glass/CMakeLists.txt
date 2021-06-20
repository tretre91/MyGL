config_files(magnifying_glass.cpp)

add_executable(magnifying_glass ${SOURCE_OUT})

set(RESOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/resources)
config_target(magnifying_glass INSTALL_SOURCE_OUT RESOURCES_DIR)