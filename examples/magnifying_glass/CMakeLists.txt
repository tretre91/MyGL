# set(SOURCE_IN ${CMAKE_CURRENT_LIST_DIR}/magnifying_glass.cpp)
# set(SOURCE_OUT ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
# set(RESSOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/ressources)
# configure_file(${SOURCE_IN} ${SOURCE_OUT} @ONLY)

# set(INSTALL_SOURCE_OUT ${CMAKE_CURRENT_BINARY_DIR}/magnifying_glass.cpp)
# set(RESSOURCES_DIR ressources)
# configure_file(${SOURCE_IN} ${INSTALL_SOURCE_OUT} @ONLY)

config_files(magnifying_glass.cpp)

add_executable(magnifying_glass ${SOURCE_OUT})

set(RESSOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/ressources)
config_target(magnifying_glass INSTALL_SOURCE_OUT RESSOURCES_DIR)