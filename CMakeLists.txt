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
  "src/Color.cpp"
  "src/Shader.cpp"
  "src/stb_image.cpp"
  "src/Texture.cpp"
  "src/Vector.cpp"
  "src/Window.cpp"
  #"src/glad.c"
  "src/Drawable/AbstractShape.cpp"
  "src/Drawable/Font.cpp"
  "src/Drawable/Rectangle.cpp"
  "src/Drawable/Text.cpp"
  "src/Camera/Cam2D.cpp"
  #"src/Camera/Cam3D.cpp"
  "src/Camera/FixedCamera.cpp"
  #"src/Camera/FpsCamera.cpp"
  "src/Camera/MovableCamera.cpp"
)

option(
  MYGL_BUILD_DOCUMENTATION 
  "Turn on in order to enable a build target for the documentation, this requires that you have doxygen, sphinx, breathe and the sphinx_rtd_theme installed"
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

# Search for the dependancies
add_subdirectory(external)

if(MYGL_SHARED AND MYGL_STATIC)
  set(MYGL_TARGETS "MyGL" "GLAD" "MyGL-static")
elseif(MYGL_SHARED)
  set(MYGL_TARGETS "MyGL" "GLAD")
elseif(MYGL_STATIC)
  set(MYGL_TARGETS "MyGL-static")
else()
  message(FATAL_ERROR "Nothing to build, enable at least one of the options MYGL_STATIC and MYGL_SHARED")
endif()

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Shared library
if(MYGL_SHARED)
  add_library(GLAD STATIC "${PROJECT_SOURCE_DIR}/src/glad.c")
  set_target_properties(GLAD PROPERTIES
    OUTPUT_NAME glad
  )
  target_compile_definitions(GLAD
    PUBLIC GLAD_GLAPI_EXPORT
    PRIVATE GLAD_GLAPI_EXPORT_BUILD
  )
  target_include_directories(GLAD
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )

  add_library(MyGL SHARED ${PROJECT_SOURCES})
  set_target_properties(MyGL PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME mygl
    DEBUG_POSTFIX "d"
  )

  target_link_libraries(MyGL
    PUBLIC GLAD
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2>
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2main>,SDL2::SDL2main,SDL2main>
    PUBLIC freetype
  )

  target_include_directories(MyGL 
    PUBLIC
      $<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>
      $<INSTALL_INTERFACE:include/SDL2>
    PUBLIC
      $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIR_ft2build}>
      $<INSTALL_INTERFACE:include/freetype2>
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>  
  )
endif(MYGL_SHARED)

# Static Library
if(MYGL_STATIC)
  add_library(MyGL-static STATIC "src/glad.c" ${PROJECT_SOURCES})
  set_target_properties(MyGL-static PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME mygl-static
    DEBUG_POSTFIX "d"
  )

  target_compile_definitions(MyGL-static PUBLIC MYGL_STATIC_DEFINE)
  
  set(SDL_TARGET $<IF:$<PLATFORM_ID:Linux>,SDL2,SDL2-static>)
  target_link_libraries(MyGL-static
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2::${SDL_TARGET}>,SDL2::${SDL_TARGET},${SDL_TARGET}>
    PUBLIC $<IF:$<TARGET_EXISTS:SDL2::SDL2main>,SDL2::SDL2main,SDL2main>
    PUBLIC freetype
  )
  unset(SDL_TARGET)

  target_include_directories(MyGL-static
    PUBLIC
      $<BUILD_INTERFACE:${SDL2_INCLUDE_DIRS}>
      $<INSTALL_INTERFACE:include/SDL2>
    PUBLIC
      $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIR_ft2build}>
      $<INSTALL_INTERFACE:include/freetype2>
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

install(DIRECTORY ${SDL2_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIR_freetype2} ${INCLUDE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


# Other targets
if(MYGL_BUILD_DOCUMENTATION)
  # FindSphinx
  set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
  add_subdirectory("docs")
endif()

if(MYGL_BUILD_SAMPLE)
  add_subdirectory("test")
endif()