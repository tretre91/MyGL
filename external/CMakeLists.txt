find_package(GLFW3 CONFIG)

if(NOT GLFW3_INCLUDE_DIR)
  message(STATUS "Unable to find GLFW3, building it")
  message(STATUS "Downloading GLFW3's sources")
  execute_process(
    COMMAND git submodule update --init -- glfw/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  message(STATUS "Download complete")
  set(GLFW3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glfw/include")
  set(USE_OWN_GLFW3 True CACHE BOOL "Tells if the submodule GLFW3 should be used (if no valid GLFW3 has been found on this system)")
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
elseif(NOT EXISTS USE_OWN_GLFW3)
  set(USE_OWN_GLFW3 False CACHE BOOL "Tells if the submodule GLFW3 should be used (if no valid GLFW3 has been found on this system)")
endif()

set(GLFW3_INCLUDE_DIR ${GLFW3_INCLUDE_DIR} CACHE PATH "Path to GLFW3 headers")
if(USE_OWN_GLFW3)
  add_subdirectory(glfw)
endif()


find_package(freetype2 CONFIG)

if((NOT FREETYPE_INCLUDE_DIR_ft2build) OR (NOT EXISTS ${FREETYPE_INCLUDE_DIR_ft2build}))
  message(STATUS "Unable to find freetype2, building it")
  message(STATUS "Downloading freetype2's sources")

  execute_process(
    COMMAND git submodule update --init -- freetype2/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  message(STATUS "Download complete")
  set(FREETYPE_INCLUDE_DIR_ft2build "${CMAKE_CURRENT_SOURCE_DIR}/freetype2/include")
  set(USE_OWN_FREETYPE True CACHE BOOL "Tells if the submodule FREETYPE should be used (if no valid FREETYPE has been found on this system)")
elseif(NOT EXISTS USE_OWN_FREETYPE)
  set(USE_OWN_FREETYPE False CACHE BOOL "Tells if the submodule FREETYPE should be used (if no valid FREETYPE has been found on this system)")
endif()

set(FREETYPE_INCLUDE_DIR_ft2build ${FREETYPE_INCLUDE_DIR_ft2build} CACHE PATH "Path to freetype headers")
if(USE_OWN_FREETYPE)
  if(UNIX AND MYGL_SHARED)
    set(BUILD_SHARED_LIBS True)
  endif()
  add_subdirectory(freetype2)
endif()