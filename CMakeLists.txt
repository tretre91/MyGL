cmake_minimum_required(VERSION 3.14)

project(
  MyGL
  LANGUAGES CXX C
  VERSION 0.1.0
  DESCRIPTION "A little graphics library using opengl, its purpose is to test my coding skills / learn opengl"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

set(PROJECT_SOURCES
    "src/glad.c"
    "src/Color.cpp"
    "src/Shader.cpp"
    "src/ShaderProgram.cpp"
    "src/stb_image.cpp"
    "src/Texture.cpp"
    "src/Window.cpp"
    "src/Drawable/AbstractShape.cpp"
    "src/Drawable/Rectangle.cpp"
    "src/Drawable/Polygon.cpp"
    "src/Drawable/Triangle.cpp"
    "src/Drawable/Font.cpp"
    "src/Drawable/Text.cpp"
    "src/Camera.cpp"
    "src/Event.cpp"
    "src/Cursor.cpp"
    "src/common.cpp"
    "src/Image.cpp")

option(MYGL_DOCUMENTATION "Turn on in order to enable a build target for the documentation, this requires that you have doxygen installed"
       OFF
)

option(MYGL_EXAMPLES "Builds the example programs" OFF)

option(MYGL_SHARED "Builds the dynamic library files. At least one of the options MYGL_SHARED and MYGL_STATIC should be enabled" ON)

option(MYGL_STATIC "Builds the dynamic library files. At least one of the options MYGL_SHARED and MYGL_STATIC should be enabled" ON)

if(UNIX AND MYGL_SHARED)
  set(BUILD_SHARED_LIBS True)
endif()

if(MYGL_SHARED)
  list(APPEND MYGL_TARGETS "MyGL")
endif()
if(MYGL_STATIC)
  list(APPEND MYGL_TARGETS "MyGL-static")
endif()
if(NOT MYGL_DOCUMENTATION AND NOT MYGL_TARGETS)
  message(FATAL_ERROR "Nothing to build, enable at least one of the options MYGL_STATIC, MYGL_SHARED or MYGL_DOCUMENTATION")
endif()

if(MYGL_TARGETS)
  include(cmake/CPM.cmake)

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

  CPMAddPackage(
    NAME utf8cpp
    GIT_REPOSITORY "https://github.com/nemtrif/utfcpp.git"
    GIT_TAG v3.2.1
    OPTIONS "UTF8_INSTALL ON"
  )
endif()

# Shared library
if(MYGL_SHARED)
  add_library(MyGL SHARED ${PROJECT_SOURCES})
  set_target_properties(
    MyGL
    PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR}
               VERSION ${PROJECT_VERSION}
               OUTPUT_NAME mygl
               DEBUG_POSTFIX "d"
               CXX_VISIBILITY_PRESET hidden
               VISIBILITY_INLINES_HIDDEN YES
  )

  target_compile_definitions(
    MyGL
    PUBLIC GLAD_GLAPI_EXPORT
    PRIVATE GLAD_GLAPI_EXPORT_BUILD
  )

  target_link_libraries(
    MyGL
    PUBLIC glfw
    PUBLIC freetype
    PUBLIC utf8::cpp
  )

  if(WIN32)
    target_link_libraries(MyGL PUBLIC winmm)
  endif()

  target_include_directories(MyGL PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
endif(MYGL_SHARED)

# Static Library
if(MYGL_STATIC)
  add_library(MyGL-static STATIC ${PROJECT_SOURCES})
  set_target_properties(MyGL-static PROPERTIES VERSION ${PROJECT_VERSION} DEBUG_POSTFIX "d")

  if(UNIX)
    set_target_properties(MyGL-static PROPERTIES OUTPUT_NAME mygl)
  else()
    set_target_properties(MyGL-static PROPERTIES OUTPUT_NAME mygl-static)
    target_link_libraries(MyGL-static PUBLIC winmm)
  endif()

  target_compile_definitions(MyGL-static PUBLIC MYGL_STATIC_DEFINE)

  target_link_libraries(
    MyGL-static
    PUBLIC glfw
    PUBLIC freetype
    PUBLIC utf8::cpp
  )

  target_include_directories(MyGL-static PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
endif(MYGL_STATIC)

# install directives
include(GNUInstallDirs)

if(MYGL_TARGETS)
  set(MYGL_INSTALL_CMAKEDIR
      ${CMAKE_INSTALL_LIBDIR}/cmake/MyGL
      CACHE STRING "Path to MyGL cmake files"
  )

  set(MYGL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

  install(
    TARGETS ${MYGL_TARGETS}
    EXPORT MyGL_Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT MyGL_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT MyGL_Runtime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT MyGL_Development
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    EXPORT MyGL_Targets
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

  install(FILES "cmake/MyGLConfig.cmake" "${CMAKE_BINARY_DIR}/MyGLConfigVersion.cmake" DESTINATION ${MYGL_INSTALL_CMAKEDIR})

  install(DIRECTORY ${MYGL_INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# Documentation directory
if(MYGL_DOCUMENTATION)
  add_subdirectory("docs")
endif()

# Example programs directory
if(MYGL_EXAMPLES)
  if(MYGL_TARGETS)
    add_subdirectory("examples")
  else()
    message(FATAL_ERROR "In order to generate the examples, you should also build the library (enable MYGL_STATIC and/or MYGL_SHARED).")
  endif()
endif()
