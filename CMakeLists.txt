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

option(
  MYGL_SHARED
  "Builds the dynamic library files. At least one of the options MYGL_SHARED and MYGL_STATIC should be enabled"
  ON
)

option(
  MYGL_STATIC
  "Builds the dynamic library files. At least one of the options MYGL_SHARED and MYGL_STATIC should be enabled"
  ON
)

# Search for the dependancies
add_subdirectory(external)


#############################

if(MYGL_SHARED AND MYGL_STATIC)
  set(MYGL_TARGETS "MyGL" "MyGL-static")
elseif(MYGL_SHARED)
  set(MYGL_TARGETS "MyGL")
elseif(MYGL_STATIC)
  set(MYGL_TARGETS "MyGL-static")
else()
  message(FATAL_ERROR "Nothing to build, enable at least one of the options MYGL_STATIC and MYGL_SHARED")
endif()

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Include)

# Shared library
if(MYGL_SHARED)
  add_library(MyGL SHARED ${PROJECT_SOURCES})
  set_target_properties(MyGL PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME mygl
    DEBUG_POSTFIX "d"
  )
  
  target_compile_definitions(MyGL
    PRIVATE GLAD_GLAPI_EXPORT_BUILD 
    PRIVATE GLAD_GLAPI_EXPORT
  )

  target_link_libraries(MyGL
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2>
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2main>,SDL2::SDL2main,SDL2main>
    INTERFACE freetype
  )

  target_include_directories(MyGL 
    # PUBLIC
    #   $<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>
    #   $<INSTALL_INTERFACE:include/SDL2>
    # PUBLIC
    #   $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIR_ft2build}>
    #   $<INSTALL_INTERFACE:include/freetype2>
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Include>
      $<INSTALL_INTERFACE:include>  
  )
endif(MYGL_SHARED)

# Static Library
if(MYGL_STATIC)
  add_library(MyGL-static STATIC ${PROJECT_SOURCES})
  set_target_properties(MyGL-static PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME mygl-static
    DEBUG_POSTFIX "d"
  )

  target_compile_definitions(MyGL-static PUBLIC MYGL_STATIC_DEFINE)
 
  target_link_libraries(MyGL-static
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2-static>,SDL2::SDL2-static,SDL2-static>
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2main>,SDL2::SDL2main,SDL2main>
    PRIVATE freetype
  )

  target_include_directories(MyGL-static
    PUBLIC
      $<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>
      $<INSTALL_INTERFACE:include/SDL2>
    PUBLIC
      $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIR_ft2build}>
      $<INSTALL_INTERFACE:include/freetype2>
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Include>
      $<INSTALL_INTERFACE:include>  
  )
endif(MYGL_STATIC)


#install directives
include(GNUInstallDirs)

set(MYGL_INSTALL_CMAKEDIR
  ${CMAKE_INSTALL_LIBDIR}/cmake/MyGL
  CACHE STRING "Path to MyGL cmake files"
)

install(TARGETS ${MYGL_TARGETS}
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