
find_package(SDL2 CONFIG)

if(NOT SDL2_INCLUDE_DIRS)
  message(STATUS "Unable to find SDL2, building it")
  message(STATUS "Downloading SDL2's sources")
  execute_process(
    COMMAND git submodule update --init -- SDL/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  message(STATUS "Download complete")
  set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/SDL/include")
  set(USE_OWN_SDL2 True CACHE BOOL "Tells if the submodule SDL2 should be used (if no valid SDL2 has been found on this system)")
elseif(NOT EXISTS USE_OWN_SDL2)
  set(USE_OWN_SDL2 False CACHE BOOL "Tells if the submodule SDL2 should be used (if no valid SDL2 has been found on this system)")
endif()

set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} CACHE PATH "Path to SDL2 headers")
if(USE_OWN_SDL2)
  add_subdirectory(SDL)
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
  add_subdirectory(freetype2)
endif()