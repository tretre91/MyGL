include(CMakeFindDependencyMacro)
find_dependency(GLFW3 PATHS "../glfw3")
find_dependency(Freetype 2 PATHS "../freetype")
include("${CMAKE_CURRENT_LIST_DIR}/MyGLTargets.cmake")