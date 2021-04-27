cmake_minimum_required(VERSION 3.14)

project(
  MyGL
  LANGUAGES CXX C
  VERSION 0.1.0 
  DESCRIPTION "A little graphics library using opengl, it is not in any way efficient as its purpose is to test my coding skills / learn opengl"
)

set(CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

set(PROJECT_SOURCES
  "src/glad.c"
  "src/Color.cpp"
  "src/Shader.cpp"
  "src/stb_image.cpp"
  "src/Texture.cpp"
  "src/Animation.cpp"
  "src/Window.cpp"
  "src/Drawable/AbstractShape.cpp"
  "src/Drawable/Rectangle.cpp"
  "src/Drawable/Polygon.cpp"
  "src/Drawable/Font.cpp"
  "src/Drawable/Text.cpp"
  "src/Camera.cpp"
  "src/Event.cpp")

option(
  MYGL_BUILD_DOCUMENTATION 
  "Turn on in order to enable a build target for the documentation, this requires that you have doxygen installed"
  OFF
)

option(
  MYGL_BUILD_SAMPLE
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

# Search for the dependencies
include(cmake/CPM.cmake)

if(UNIX AND MYGL_SHARED)
  set(BUILD_SHARED_LIBS True)
endif()

CPMAddPackage(
  NAME glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG 3.3-stable
  OPTIONS "GLFW_BUILD_DOCS OFF" "GLFW_BUILD_TESTS OFF" "GLFW_BUILD_EXAMPLES OFF"
)

CPMAddPackage(
  NAME freetype
  GIT_REPOSITORY "https://gitlab.freedesktop.org/freetype/freetype.git"
  GIT_TAG VER-2-10-4
)


if(MYGL_SHARED)
  list(APPEND MYGL_TARGETS "MyGL")
endif()
if(MYGL_STATIC)
  list(APPEND MYGL_TARGETS "MyGL-static")
endif()
if(NOT MYGL_BUILD_DOCUMENTATION AND NOT MYGL_TARGETS)
  message(FATAL_ERROR "Nothing to build, enable at least one of the options MYGL_STATIC, MYGL_SHARED or MYGL_BUILD_DOCUMENTATION")
endif()

set(MYGL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Shared library
if(MYGL_SHARED)
  add_library(MyGL SHARED ${PROJECT_SOURCES})
  set_target_properties(MyGL PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME mygl
    DEBUG_POSTFIX "d"
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
  )

  target_compile_definitions(MyGL
    PUBLIC GLAD_GLAPI_EXPORT
    PRIVATE GLAD_GLAPI_EXPORT_BUILD)

  target_link_libraries(MyGL
    PUBLIC glfw
    PUBLIC freetype
  )

  target_include_directories(MyGL 
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )
endif(MYGL_SHARED)

# Static Library
if(MYGL_STATIC)
  add_library(MyGL-static STATIC ${PROJECT_SOURCES})
  set_target_properties(MyGL-static PROPERTIES
    VERSION ${PROJECT_VERSION}
    DEBUG_POSTFIX "d"
  )

  if(UNIX)
    set_target_properties(MyGL-static PROPERTIES OUTPUT_NAME mygl)
  else()
    set_target_properties(MyGL-static PROPERTIES OUTPUT_NAME mygl-static)
  endif()

  target_compile_definitions(MyGL-static PUBLIC MYGL_STATIC_DEFINE)
  
  target_link_libraries(MyGL-static
    PUBLIC glfw
    PUBLIC freetype
  )

  target_include_directories(MyGL-static
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
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

install(FILES "cmake/MyGLConfig.cmake" "${CMAKE_BINARY_DIR}/MyGLConfigVersion.cmake"
  DESTINATION ${MYGL_INSTALL_CMAKEDIR}
)

install(
  DIRECTORY 
    ${MYGL_INCLUDE_DIR}/
  DESTINATION 
    ${CMAKE_INSTALL_INCLUDEDIR})

# Documentation target
if(MYGL_BUILD_DOCUMENTATION)
  add_subdirectory("docs")
endif()

if(MYGL_BUILD_SAMPLE)
  add_subdirectory("test")
endif()