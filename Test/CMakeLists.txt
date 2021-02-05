set(TEST_SOURCE_IN main.cpp)
set(TEST_SOURCE_OUT ${CMAKE_BINARY_DIR}/Test/test.cpp)
set(RESSOURCES_DIR ${PROJECT_SOURCE_DIR}/Ressources)

configure_file(${TEST_SOURCE_IN} ${TEST_SOURCE_OUT} @ONLY)
add_executable(TestFile ${TEST_SOURCE_OUT})

set_target_properties(
    TestFile PROPERTIES
    OUTPUT_NAME testGL
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Test
)

target_include_directories(TestFile PUBLIC ${INCLUDE_DIR})

if(UNIX)
    target_link_libraries(TestFile MyGL GL pthread X11 dl Xrandr udev)
elseif(WIN32)
    #target_link_libraries(TestFile ${TARGET_LIB_PATH})
    target_link_libraries(TestFile MyGL)
else()
    target_link_libraries(TestFile MyGL)
endif()