set(TEST_SOURCE_IN main.cpp)

set(TEST_SOURCE_INSTALL ${CMAKE_BINARY_DIR}/Test/example.cpp)
set(RESSOURCES_DIR "Ressources")
configure_file(${TEST_SOURCE_IN} ${TEST_SOURCE_INSTALL} @ONLY)

set(TEST_SOURCE_OUT ${CMAKE_BINARY_DIR}/Test/test.cpp)
set(RESSOURCES_DIR ${PROJECT_SOURCE_DIR}/Ressources)
configure_file(${TEST_SOURCE_IN} ${TEST_SOURCE_OUT} @ONLY)

add_executable(TestFile ${TEST_SOURCE_OUT})

set_target_properties(TestFile PROPERTIES
  OUTPUT_NAME testGL
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Test
)

target_include_directories(TestFile 
  PUBLIC
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
  PUBLIC
    $<BUILD_INTERFACE:${SDL2_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include/SDL2>
  PUBLIC
    $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIR_freetype2}>
    $<INSTALL_INTERFACE:include/freetype2>
)

target_link_libraries(TestFile MyGL-static SDL2 freetype)
if(UNIX)
  target_link_libraries(TestFile dl)
endif()

# Install directives

set(MYGL_EXAMPLE_DIR "example")

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