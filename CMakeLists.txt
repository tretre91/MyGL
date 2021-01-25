cmake_minimum_required(VERSION 3.10)

project(
  MyGL 
  VERSION 0.1.0 
  DESCRIPTION "A little graphics library using opengl, it is not in any way efficient as its purpose is to test my coding skills / learn opengl"
)

set(CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(RELEASE "Compile release binaries, this option is set to OFF by default (which will build debug binaries)" OFF)
option(BUILD_SHARED_LIBS "Copmpile as a shared library (.dll on windows, .so on linux), OFF by default " OFF)

if(RELEASE)
  set(CMAKE_BUILD_TYPE "Release")
  set(TARGET_LIB_NAME "mygl")
  set(LIB_NAMES "freetype" "sfml-window-s" "sfml-system-s")
  set(TEMP_LIB_NAME "temp")
else ()
  set(CMAKE_BUILD_TYPE "Debug")
  set(TARGET_LIB_NAME "mygld")
  set(LIB_NAMES "freetyped" "sfml-window-s-d" "sfml-system-s-d")
  set(TEMP_LIB_NAME "tempd")
endif()

if(WIN32)
    set(LIB_FOLDER ${PROJECT_SOURCE_DIR}/Libs/Windows)
    set(LIB_NAMES ${LIBNAMES}  "opengl32" "winmm")
    foreach(LIB_NAME ${LIB_NAMES})
        set(LIBS ${LIBS} ${LIB_NAME}.lib)
    endforeach()
    message(WARNING ${LIBS}) 
    set(TEMP_LIB_NAME ${CMAKE_BINARY_DIR}/${TEMP_LIB_NAME}.lib)
    set(TARGET_LIB_NAME ${TARGET_LIB_NAME}.lib)
    set(TARGET_LIB_PATH ${CMAKE_BINARY_DIR}/${TARGET_LIB_NAME})
else() #if(UNIX)
    set(LIB_FOLDER ${PROJECT_SOURCE_DIR}/Libs/Unix)
    foreach(LIB_NAME ${LIB_NAMES})
        set(LIBS ${LIBS} lib${LIB_NAME}.a)
    endforeach()
    set(TEMP_LIB_NAME ${CMAKE_BINARY_DIR}/lib${TEMP_LIB_NAME}.a)
    set(TARGET_LIB_NAME lib${TARGET_LIB_NAME}.a)
    set(TARGET_LIB_PATH ${CMAKE_BINARY_DIR}/${TARGET_LIB_NAME})
endif()
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Include)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/Source)


set(PROJECT_SOURCES
    "Source/Color.cpp"
    "Source/Shader.cpp"
    "Source/stb_image.cpp"
    "Source/Texture.cpp"
    "Source/Vector.cpp"
    "Source/Window.cpp"
    "Source/glad.c"
    "Source/Drawable/AbstractShape.cpp"
    "Source/Drawable/Font.cpp"
    "Source/Drawable/Rectangle.cpp"
    "Source/Drawable/Text.cpp"
    "Source/Camera/Cam2D.cpp"
    "Source/Camera/Cam3D.cpp"
    "Source/Camera/FixedCamera.cpp"
    "Source/Camera/FpsCamera.cpp"
    "Source/Camera/MovableCamera.cpp"
)

add_library(temp ${PROJECT_SOURCES})
set_target_properties(temp PROPERTIES DEBUG_POSTFIX "d")
target_include_directories(temp PUBLIC "${PROJECT_SOURCE_DIR}/Include")


if(MSVC)
    set(JOIN_COMMAND lib.exe /NOLOGO /OUT:${TARGET_LIB_PATH} ${TEMP_LIB_NAME} ${LIBS})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") #Mac OS X
    set(JOIN_COMMAND libtool -static -o ${TARGET_LIB_PATH} ${TEMP_LIB_NAME} ${LIBS})
else()
    set(MRI_FILE "${CMAKE_BINARY_DIR}/${TARGET_LIB_NAME}.mri")
    file(WRITE ${MRI_FILE} "create ${TARGET_LIB_PATH}\n")
    file(APPEND ${MRI_FILE} "addlib ${TEMP_LIB_NAME}\n")
    foreach(LIBRARY ${LIBS})
        file(APPEND ${MRI_FILE} "addlib ${LIBRARY}\n")
    endforeach()
    file(APPEND ${MRI_FILE} "save\n")
    file(APPEND ${MRI_FILE} "end\n")

    set(JOIN_COMMAND ar -M < ${MRI_FILE})
endif()

add_custom_command(
    TARGET temp POST_BUILD
    COMMAND ${JOIN_COMMAND}
    WORKING_DIRECTORY ${LIB_FOLDER}
    VERBATIM
)

# FindSphinx
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
# To build the documenation
add_subdirectory("Docs")

# To build the test program
add_subdirectory("Test")