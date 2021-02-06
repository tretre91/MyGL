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

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

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
  #"Source/Camera/Cam3D.cpp"
  "Source/Camera/FixedCamera.cpp"
  #"Source/Camera/FpsCamera.cpp"
  "Source/Camera/MovableCamera.cpp"
)

# option(
#   BUILD_SHARED_LIBS
#   "Compile as a shared library (.dll on windows, .so on linux), OFF by default"
#   OFF
# )

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

set(SDL_BUILDING_LIBRARY TRUE)
find_package(SDL2)
find_package(Freetype 2)

if(NOT SDL2_FOUND)
  find_package(SDL2 REQUIRED 
    PATHS ${PROJECT_SOURCE_DIR}/Libs/
  )
endif()

if(NOT FREETYPE_FOUND)
  find_package(Freetype 2 REQUIRED
    PATHS ${PROJECT_SOURCE_DIR}/Libs/
  )
endif()

# Shared library
add_library(MyGL SHARED ${PROJECT_SOURCES})
set_target_properties(MyGL PROPERTIES
  SOVERSION ${PROJECT_VERSION_MAJOR}
  VERSION ${PROJECT_VERSION}
  OUTPUT_NAME mygl
  DEBUG_POSTFIX "d"
)

add_library(GLAD SHARED "${PROJECT_SOURCE_DIR}/Source/glad.c")
target_include_directories(GLAD 
  PRIVATE "${PROJECT_SOURCE_DIR}/Include"
)
target_compile_definitions(GLAD
  PRIVATE GLAD_GLAPI_EXPORT_BUILD 
  PRIVATE GLAD_GLAPI_EXPORT
)
set_target_properties(GLAD PROPERTIES
  OUTPUT_NAME glad
)

target_link_libraries(MyGL 
  PUBLIC GLAD
  PUBLIC SDL2
  PUBLIC SDL2main
  PRIVATE freetype
)

# Static Library
add_library(MyGL-static STATIC ${PROJECT_SOURCES})
set_target_properties(MyGL-static PROPERTIES
  VERSION ${PROJECT_VERSION}
  OUTPUT_NAME mygl-static
  DEBUG_POSTFIX "d"
)

target_compile_definitions(MyGL-static PUBLIC MYGL_STATIC_DEFINE)

if(WIN32)
  target_link_libraries(MyGL PUBLIC SDL2-static)
else()
  target_link_libraries(MyGL PUBLIC SDL2)
endif()
target_link_libraries(MyGL
  PUBLIC SDL2main
  PRIVATE freetype
)

#include(GenerateExportHeader)
#generate_export_header(MyGL)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Include)

target_include_directories(MyGL 
  PUBLIC
    $<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include/SDL2>
  PRIVATE
    ${FREETYPE_INCLUDE_DIR_freetype2}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include>  
)


target_include_directories(MyGL-static
  PUBLIC
    $<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:include/SDL2>
  PRIVATE
    ${FREETYPE_INCLUDE_DIR_freetype2}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include>  
)

#install directives
include(GNUInstallDirs)

set(MYGL_INSTALL_CMAKEDIR
  ${CMAKE_INSTALL_LIBDIR}/cmake/MyGL
  CACHE STRING "Path to MyGL cmake files"
)

install(TARGETS MyGL MyGL-static GLAD
  EXPORT MyGL_Targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          COMPONENT MyGL_Runtime
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT MyGL_Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT MyGL_Development
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT MyGL_Targets
  FILE MyGLTargets.cmake
  NAMESPACE MyGL::
  DESTINATION ${MYGL_INSTALL_CMAKEDIR}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  MyGLConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES "CMake/MyGLConfig.cmake" "${CMAKE_BINARY_DIR}/MyGLConfigVersion.cmake"
  DESTINATION ${MYGL_INSTALL_CMAKEDIR}
)

install(DIRECTORY ${SDL2_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIR_freetype2} ${INCLUDE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


# Other targets

if(BUILD_DOCUMENTATION)
  # FindSphinx
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
  add_subdirectory("Docs")
endif()

if(BUILD_SAMPLE)
  add_subdirectory("Test")
endif()