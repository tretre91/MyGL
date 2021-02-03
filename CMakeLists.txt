cmake_minimum_required(VERSION 3.10)

project(
    MyGL
    LANGUAGES CXX C
    VERSION 0.1.0 
    DESCRIPTION "A little graphics library using opengl, it is not in any way efficient as its purpose is to test my coding skills / learn opengl"
)

set(CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)


set(PROJECT_SOURCES
    "Source/Color.cpp"
    "Source/Shader.cpp"
    "Source/stb_image.cpp"
    "Source/Texture.cpp"
    "Source/Vector.cpp"
    "Source/Window.cpp"
    "Source/glad.c"
    "Source/test.c"
    "Source/Drawable/AbstractShape.cpp"
    "Source/Drawable/Font.cpp"
    "Source/Drawable/Rectangle.cpp"
    "Source/Drawable/Text.cpp"
    "Source/Camera/Cam2D.cpp"
    #"Source/Camera/Cam3D.cpp"
    "Source/Camera/FixedCamera.cpp"
    #"Source/Camera/FpsCamera.cpp"
    "Source/Camera/MovableCamera.cpp"
)

set(LIB_HEADERS
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Color.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Shader.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/stb_image.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Texture.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Vector.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Window.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/glad.h"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/test.h"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Drawable/AbstractShape.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Drawable/Rectangle.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Drawable/Polygon.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Drawable/Font.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Drawable/Text.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Camera/Cam2D.hpp"
    #"${PROJECT_SOURCE_DIR}/Include/MyGL/Camera/Cam3D.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Camera/FixedCamera.hpp"
    #"${PROJECT_SOURCE_DIR}/Include/MyGL/Camera/FpsCamera.hpp"
    "${PROJECT_SOURCE_DIR}/Include/MyGL/Camera/MovableCamera.hpp"
)

option(BUILD_SHARED_LIBS "Compile as a shared library (.dll on windows, .so on linux), OFF by default " OFF)

add_library(MyGL ${PROJECT_SOURCES})
#target_sources(MyGL PUBLIC "${PROJECT_SOURCE_DIR}/Source/glad.c" )
set_target_properties(MyGL PROPERTIES
  #PUBLIC_HEADER ${LIB_HEADERS}
  SOVERSION 0
)

include(GNUInstallDirs)
include(GenerateExportHeader)
generate_export_header(MyGL)

target_include_directories(MyGL PUBLIC "${PROJECT_SOURCE_DIR}/Include")

set(SDL_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED
  PATHS ${PROJECT_SOURCE_DIR}/Libs/
)

find_package(freetype 2 REQUIRED
  PATHS ${PROJECT_SOURCE_DIR}/Libs/
)

if(NOT SDL2_FOUND)
  message(ERROR "Failed to find SDL2 libs")
elseif(NOT FREETYPE_FOUND)
  message(ERROR "Failed to find freetype libs")
endif()


if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    set(CMAKE_BUILD_TYPE "Debug")
    set(TARGET_LIB_NAME "mygld")
    set(LIB_NAMES "freetyped" "SDL2-staticd")
    set(TEMP_LIB_NAME "tempd")
else()
    set(CMAKE_BUILD_TYPE "Release")
    set(TARGET_LIB_NAME "mygl")
    set(LIB_NAMES "freetype" "SDL2-static")
    set(TEMP_LIB_NAME "temp")
endif()

if(WIN32)
    set(LIB_FOLDER ${PROJECT_SOURCE_DIR}/Libs/Windows)
    set(LIB_NAMES ${LIB_NAMES} "opengl32" "winmm")
    foreach(LIB_NAME ${LIB_NAMES})
        set(LIBS ${LIBS} ${LIB_NAME}.lib)
    endforeach()
    set(TEMP_LIB_NAME ${CMAKE_BINARY_DIR}/${TEMP_LIB_NAME}.lib)
    string(REPLACE "/" "\\" TEMP_LIB_NAME ${TEMP_LIB_NAME})
    set(TARGET_LIB_NAME ${TARGET_LIB_NAME}.lib)
    set(TARGET_LIB_PATH ${CMAKE_BINARY_DIR}/${TARGET_LIB_NAME})
    set(DELETE_COMMAND del)
else() #if(UNIX)
    set(LIB_FOLDER ${PROJECT_SOURCE_DIR}/Libs/Unix)
    foreach(LIB_NAME ${LIB_NAMES})
        set(LIBS ${LIBS} lib${LIB_NAME}.a)
    endforeach()
    set(TEMP_LIB_NAME ${CMAKE_BINARY_DIR}/lib${TEMP_LIB_NAME}.a)
    set(TARGET_LIB_NAME lib${TARGET_LIB_NAME}.a)
    set(TARGET_LIB_PATH ${CMAKE_BINARY_DIR}/${TARGET_LIB_NAME})
    set(DELETE_COMMAND rm)
endif()
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Include)
set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/Source)


if(BUILD_SHARED_LIBS)
    add_library(GLAD "${PROJECT_SOURCE_DIR}/Source/glad.c")
    target_include_directories(GLAD PUBLIC "${PROJECT_SOURCE_DIR}/Include")
    #target_link_libraries(GLAD PUBLIC opengl32 PUBLIC winmm )
    target_compile_definitions(GLAD PRIVATE GLAD_GLAPI_EXPORT_BUILD PRIVATE GLAD_GLAPI_EXPORT)
    target_link_libraries(MyGL 
      PUBLIC GLAD
      PUBLIC SDL2::SDL2
      PUBLIC SDL2::SDL2main
      PRIVATE freetype
    )
    target_compile_definitions(MyGL PUBLIC SDL_BUILDING_LIBRARY)
    set_target_properties(
        MyGL PROPERTIES
        DEBUG_POSTFIX "d"
    )
else()
    
    set_target_properties(
        MyGL PROPERTIES
        DEBUG_POSTFIX "d"
        OUTPUT_NAME temp
    )

    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DMYGL_STATIC_DEFINE")
        set(JOIN_COMMAND lib.exe /NOLOGO /OUT:${TARGET_LIB_PATH} ${TEMP_LIB_NAME} ${LIBS})
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") #Mac OS X
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMYGL_STATIC_DEFINE")
        set(JOIN_COMMAND libtool -static -o ${TARGET_LIB_PATH} ${TEMP_LIB_NAME} ${LIBS})
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMYGL_STATIC_DEFINE")
        set(MRI_FILE "${CMAKE_BINARY_DIR}/${TARGET_LIB_NAME}.mri")
        file(WRITE ${MRI_FILE} "create ${TARGET_LIB_PATH}\n")
        file(APPEND ${MRI_FILE} "addlib ${TEMP_LIB_NAME}\n")
        foreach(LIBRARY ${LIBS})
            file(APPEND ${MRI_FILE} "addlib ${LIBRARY}\n")
        endforeach()
        file(APPEND ${MRI_FILE} "save\n")
        file(APPEND ${MRI_FILE} "end\n")

        set(JOIN_COMMAND ar -M < ${MRI_FILE} && ${DELETE_COMMAND} ${MRI_FILE})
    endif()

    add_custom_command(
        TARGET MyGL POST_BUILD
        COMMAND ${JOIN_COMMAND} && ${DELETE_COMMAND} ${TEMP_LIB_NAME}
        WORKING_DIRECTORY ${LIB_FOLDER}
        VERBATIM
    )

    install(FILES ${TARGET_LIB_PATH} DESTINATION lib/)
    
endif()




# FindSphinx
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
# To build the documenation
#add_subdirectory("Docs")

# To build the test program
add_subdirectory("Test")