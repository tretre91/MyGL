[![Build](https://github.com/tretre91/MyGL/actions/workflows/test_build.yml/badge.svg)](https://github.com/tretre91/MyGL/actions/workflows/test_build.yml)
[![License: Zlib](https://img.shields.io/badge/License-Zlib-lightgrey.svg)](https://opensource.org/licenses/Zlib)

# MyGL

A 2D graphics library to display basic shapes and text to a window using OpenGL!

### Disclaimer

This is a student project, I am relatively new to graphics programming so do not expect the most
efficient, optimized and well tested code. If you want an efficient graphics library there are
better solutions out there (SFML, SDL, ...).
I'm also opened to suggestions for improving this project.

## Build

This library is built using CMake, in order to build directly from the sources you have to **clone** this
repository (because dependency management uses git submodules, cloning external libs without an initialized
git repo will fail) and use CMake to build and install the binaries.


### Dependencies

This project uses several dependencies :
- [glm](https://github.com/g-truc/glm)\*
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)\*
- [glad](https://github.com/Dav1dde/glad)\*
- [glfw](https://www.glfw.org/)
- [freetype](https://www.freetype.org/)
###### \* Their sources are bundled with the source code, the other ones are installed if they are not found when configuring CMake.

----

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)