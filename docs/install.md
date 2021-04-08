# Installation

## Build the library

You can build this libary from its sources using CMake, you will need to clone
[the repository](https://github.com/tretre91/MyGL) and build with CMake.

To get the sources you absollutely need to **clone** the repository, as it uses git submodules for dependency management

```bash
git clone https://github.com/tretre91/MyGL.git
```

The main CMake options are :
- `MYGL_STATIC`: Builds the static version of the library (on by default)
- `MYGL_SHARED`: Builds the shared version of the library (on by default)
- `MYGL_BUILD_DOCUMENTATION`: Builds the documentation
- `MYGL_BUILD_SAMPLE`: Builds the file used for testing
- `CMAKE_INSTALL_PREFIX`: Sets the path to the folder where the files will be installed


## Build the documentation

In order to build the documentation you will need to have [doxygen](https://www.doxygen.nl/download.html) installed (at least v1.80) and enable the `MYGL_BUILD_DOCUMENTATION` option when generating the build files with CMake, this will create a target named 'Doxygen'