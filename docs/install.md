# Installation  {#installation}

## Build the library

You can build this libary from its sources using CMake, you will need to clone
[the repository](https://github.com/tretre91/MyGL) or download the source code
and build with CMake.

The main CMake options are :
- `MYGL_STATIC` : builds the static version of the library (default: on)
- `MYGL_SHARED` : builds the shared version of the library (default: on)
- `MYGL_DOCUMENTATION` : builds the documentation (default: off)
- `MYGL_EXAMPLES` : builds the example programs (default: off)
- `CMAKE_INSTALL_PREFIX` : sets the path to the folder where the files will be installed

@note
You might have to install with administrator privileges depending on the install directory.

This project uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) for 
dependency managment, so you can also set the options related to CPM (they are
listed [here](https://github.com/cpm-cmake/CPM.cmake#options)). The most relevant
ones are :
- `CPM_USE_LOCAL_PACKAGES` : if on, CPM will try to find a local installation
  of the dependancies before dowloading them
- `CPM_SOURCE_CACHE` : sets a directory in which the dependancies will be cached,
  it can be useful if you plan to build the library offline for example.

### Example

The installation procedure on linux should look something like this if you use
cmake from the command line (it should be similar on windows) :

```bash
git clone https://github.com/tretre91/MyGL.git
mkdir build
cd build
cmake ../MyGL -DCMAKE_BUILD_TYPE=Release -DCPM_USE_LOCAL_PACKAGES=ON
cmake --build .
cmake --install .
# or 'sudo cmake --install .' if installing in a standard location (e.g. /usr/local)
```

## Build the documentation

In order to build the documentation you will need to have [doxygen](https://www.doxygen.nl/download.html)
installed and enable the `MYGL_DOCUMENTATION` option when generating the build files with CMake.