# Modify the example source file to include the ressources path
set(TEST_SOURCE_IN main.cpp)

set(TEST_SOURCE_INSTALL ${CMAKE_BINARY_DIR}/test/example.cpp)
set(RESSOURCES_DIR "ressources")
configure_file(${TEST_SOURCE_IN} ${TEST_SOURCE_INSTALL} @ONLY)

set(TEST_SOURCE_OUT ${CMAKE_BINARY_DIR}/test/test.cpp)
set(RESSOURCES_DIR ${PROJECT_SOURCE_DIR}/ressources)
configure_file(${TEST_SOURCE_IN} ${TEST_SOURCE_OUT} @ONLY)

add_executable(TestFile ${TEST_SOURCE_OUT})

set_target_properties(TestFile PROPERTIES
  OUTPUT_NAME testGL
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)

target_link_libraries(TestFile
  PUBLIC $<IF:$<TARGET_EXISTS:MyGL-static>,MyGL-static,MyGL>
)

if(UNIX)
  target_link_libraries(TestFile 
    PUBLIC dl
    PUBLIC pthread)
endif()

target_include_directories(TestFile 
  PUBLIC
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Install directives

set(MYGL_EXAMPLE_DIR "example")

if(NOT MYGL_STATIC)
  install(TARGETS MyGL
    RUNTIME DESTINATION ${MYGL_EXAMPLE_DIR}
    LIBRARY DESTINATION ${MYGL_EXAMPLE_DIR}
  )
endif()

install(TARGETS TestFile
  RUNTIME DESTINATION ${MYGL_EXAMPLE_DIR}
)

install(DIRECTORY ${RESSOURCES_DIR}
  DESTINATION ${MYGL_EXAMPLE_DIR}
  PATTERN "*.glsl" EXCLUDE
)

install(FILES ${TEST_SOURCE_INSTALL}
  DESTINATION ${MYGL_EXAMPLE_DIR}
)