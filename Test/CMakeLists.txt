add_executable(TestFile main.cpp)

set_target_properties(
    TestFile PROPERTIES
    OUTPUT_NAME testGL
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Test
)

add_definitions(-DSFML_STATIC)

target_include_directories(TestFile PUBLIC ${INCLUDE_DIR})

if(UNIX)
    target_link_libraries(TestFile ${TARGET_LIB_PATH} GL pthread X11 dl Xrandr udev)
elseif(WIN32)
    target_link_libraries(TestFile ${TARGET_LIB_PATH})
else()
    target_link_libraries(TestFile ${TARGET_LIB_PATH})
endif()