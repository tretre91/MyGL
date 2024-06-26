[![Build](https://github.com/tretre91/MyGL/actions/workflows/build.yml/badge.svg)](https://github.com/tretre91/MyGL/actions/workflows/build.yml)
[![License: Zlib](https://img.shields.io/badge/License-Zlib-lightgrey.svg)](https://opensource.org/licenses/Zlib)

# MyGL

A 2D graphics library to display basic shapes and text to a window using OpenGL!

The documentation can be found at https://tretre91.github.io/MyGL/

### Disclaimer

This is a student project, I am relatively new to graphics programming so do not expect the most
efficient, optimized and well tested code. If you want an efficient graphics library there are
others solutions that might better suit your needs (for example SFML or SDL2).

## Build

This library is built using Meson, further information can be found [here](docs/install.md).
Note that you should have doxygen installed if you want to build the documentation.

## Dependencies

This library depends on [glfw](https://www.glfw.org/) for the window implementation
and [freetype](https://www.freetype.org/) for the font / text implementation. They
can be downloaded when configuring the project if not found on the system.

It also has smaller dependencies which are included in the source code :
- [glm](https://github.com/g-truc/glm) for the vectors and most of the mathematical
  functions
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) for texture
  loading
- [glad](https://github.com/Dav1dde/glad) for the OpenGL loader 
- [Doxygen Awesome](https://jothepro.github.io/doxygen-awesome-css/) for the
  doxygen css theme

----

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)
