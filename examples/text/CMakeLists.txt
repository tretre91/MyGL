config_file(text.cpp)

add_executable(text ${SOURCE_OUT})

set(RESOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/resources)

config_target(
  text
  FILES ${INSTALL_SOURCE_OUT}
  DIRECTORIES ${RESOURCES_DIR}
)
