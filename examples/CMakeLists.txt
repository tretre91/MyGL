set(MYGL_EXAMPLES_DIR "examples")

macro(config_file SOURCE_FILE)
  set(SOURCE_IN ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_FILE})
  set(SOURCE_OUT ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
  set(RESOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/resources)
  configure_file(${SOURCE_IN} ${SOURCE_OUT} @ONLY)

  set(INSTALL_SOURCE_OUT ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE})
  set(RESOURCES_DIR resources)
  configure_file(${SOURCE_IN} ${INSTALL_SOURCE_OUT} @ONLY)
endmacro()

function(config_target TARGET_NAME)
  set(multiValueArgs FILES DIRECTORIES)
  cmake_parse_arguments(CONFIG_TARGET "" "" "${multiValueArgs}" ${ARGN})

  target_link_libraries(${TARGET_NAME} PUBLIC $<IF:$<TARGET_EXISTS:MyGL-static>,MyGL-static,MyGL>)

  if(UNIX)
    target_link_libraries(${TARGET_NAME} PUBLIC dl)
  endif()

  target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${MYGL_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)

  set(INSTALL_DIR ${MYGL_EXAMPLES_DIR}/${TARGET_NAME})
  install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${INSTALL_DIR})

  if(NOT MYGL_STATIC AND WIN32)
    install(TARGETS MyGL RUNTIME DESTINATION ${INSTALL_DIR})
  endif()

  if(CONFIG_TARGET_FILES)
    install(FILES ${CONFIG_TARGET_FILES} DESTINATION ${INSTALL_DIR})
  endif()

  if(CONFIG_TARGET_DIRECTORIES)
    install(DIRECTORY ${CONFIG_TARGET_DIRECTORIES} DESTINATION ${INSTALL_DIR})
  endif()
endfunction()

add_subdirectory(simple_program)
add_subdirectory(text)
add_subdirectory(magnifying_glass)
add_subdirectory(custom_shader)
