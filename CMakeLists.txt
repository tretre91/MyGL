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

option(
  BUILD_SHARED_LIBS
  "Compile as a shared library (.dll on windows, .so on linux), OFF by default"
  OFF
)

option(
  BUILD_DOCUMENTATION 
  "Turn on in order to enable a build target for the documentation, this requires that you have doxygen, sphinx, breathe and the sphinx_rtd_theme installed"
  OFF
)

option(
  BUILD_SAMPLE
  "Enables a build target for a sample executable using this library (the one used for testing)"
  OFF
)

add_library(MyGL ${PROJECT_SOURCES})
set_target_properties(MyGL PROPERTIES
  SOVERSION 0
  DEBUG_POSTFIX "d"
)

include(GNUInstallDirs)
include(GenerateExportHeader)
generate_export_header(MyGL)

target_include_directories(MyGL PUBLIC "${PROJECT_SOURCE_DIR}/Include")

set(SDL_BUILDING_LIBRARY TRUE)
find_package(SDL2)

find_package(Freetype 2)

if(NOT SDL2_FOUND)
  find_package(SDL2 REQUIRED 
    PATHS ${PROJECT_SOURCE_DIR}/Libs/
  )
elseif(NOT FREETYPE_FOUND)
  find_package(Freetype 2 REQUIRED
    PATHS ${PROJECT_SOURCE_DIR}/Libs/
  )
endif()

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Include)


if(BUILD_SHARED_LIBS)
  add_library(GLAD "${PROJECT_SOURCE_DIR}/Source/glad.c")
  target_include_directories(GLAD PUBLIC "${PROJECT_SOURCE_DIR}/Include")
  target_compile_definitions(GLAD PRIVATE GLAD_GLAPI_EXPORT_BUILD PRIVATE GLAD_GLAPI_EXPORT)
  target_link_libraries(MyGL 
    PUBLIC GLAD
  )
  set_target_properties(
    MyGL PROPERTIES
    OUTPUT_NAME mygl
  )
else()
  target_compile_definitions(MyGL PUBLIC MYGL_STATIC_DEFINE)
  set_target_properties(
    MyGL PROPERTIES
    OUTPUT_NAME mygl-static
  )
endif()

target_link_libraries(MyGL
  PUBLIC SDL2::SDL2
  PUBLIC SDL2::SDL2main
  PRIVATE freetype
)

if(BUILD_DOCUMENTATION)
  # FindSphinx
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
  add_subdirectory("Docs")
endif()

if(BUILD_SAMPLE)
  add_subdirectory("Test")
endif()