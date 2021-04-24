include(CMakeFindDependencyMacro)
find_dependency(GLFW3)
find_dependency(Freetype 2)
include("${CMAKE_CURRENT_LIST_DIR}/MyGLTargets.cmake")