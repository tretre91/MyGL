include(CMakeFindDependancyMacro)
find_dependancy(SDL2)
find_dependancy(Freetype 2)
include("${CMAKE_CURRENT_LIST_DIR}/MyGLTargets.cmake")