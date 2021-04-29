set(MYGL_EXAMPLES_DIR "examples")

macro(config_files SOURCE_FILE)
  set(SOURCE_IN ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_FILE})
  set(SOURCE_OUT ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)
  set(RESSOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/ressources)
  configure_file(${SOURCE_IN} ${SOURCE_OUT} @ONLY)

  set(INSTALL_SOURCE_OUT ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE})
  set(RESSOURCES_DIR ressources)
  configure_file(${SOURCE_IN} ${INSTALL_SOURCE_OUT} @ONLY)
endmacro()

function(config_target TARGET_NAME INSTALL_FILES INSTALL_DIRECTORIES)
  target_link_libraries(${TARGET_NAME}
    PUBLIC $<IF:$<TARGET_EXISTS:MyGL-static>,MyGL-static,MyGL>)

  target_include_directories(${TARGET_NAME}
    PUBLIC
      $<BUILD_INTERFACE:${MYGL_INCLUDE_DIR}>
      $<INSTALL_INTERFACE:include>
  )

  set(INSTALL_DIR ${MYGL_EXAMPLES_DIR}/${TARGET_NAME})

  if(NOT MYGL_STATIC AND WIN32)
    install(TARGETS MyGL
      RUNTIME DESTINATION ${INSTALL_DIR})
  endif()

  install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR})

  if(${INSTALL_FILES})
    install(FILES ${${INSTALL_FILES}}
      DESTINATION ${INSTALL_DIR})
  endif()

  if(${INSTALL_DIRECTORIES})
    install(DIRECTORY ${${INSTALL_DIRECTORIES}}
      DESTINATION ${INSTALL_DIR})
  endif()
endfunction()


add_subdirectory(simple_program)
add_subdirectory(text)
add_subdirectory(magnifying_glass)