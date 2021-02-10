
find_package(SDL2 CONFIG)

if(NOT SDL2_INCLUDE_DIRS)
  message(STATUS "Unable to find SDL2, building it")
  message(STATUS "Downloading SDL2's sources")
  execute_process(
    COMMAND git submodule update --init -- SDL/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  message(STATUS "Download complete")
endif()

set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/SDL/include" CACHE PATH "Path to SDL2 headers")
add_subdirectory(SDL)

find_package(freetype2 CONFIG)

if((NOT FREETYPE_INCLUDE_DIR_ft2build) OR (NOT EXISTS ${FREETYPE_INCLUDE_DIR_ft2build}))
  message(STATUS "Unable to find freetype2, building it")
  message(STATUS "Downloading freetype2's sources")

  execute_process(
    COMMAND git submodule update --init -- freetype2/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  message(STATUS "Download complete")
endif()

set(FREETYPE_INCLUDE_DIR_ft2build "${CMAKE_CURRENT_SOURCE_DIR}/freetype2/include" CACHE PATH "Path to freetype headers")
add_subdirectory(freetype2)