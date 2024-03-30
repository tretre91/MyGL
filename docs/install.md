# Installation  {#installation}

## Build the library

You can build this libary from its sources using [Meson](https://mesonbuild.com/),
you will need to clone [the repository](https://github.com/tretre91/MyGL) or
download the source code and build with Meson.

The build options specific to this project are:
- `mygl_build_examples`: build the examples programs (default: `false`)
- `mygl_build_docs`: build the html documentation (default: `false`)
- `mygl_docs_only`: only build the documentation (default: `false`)

They are specified on the command line using the following syntax:

```bash
meson setup build -Dmygl_build_examples=true
```

Some relevant Meson options are:
- `--buildtype`: sets the build type (`debug`, `debugoptimized`, `release`, `minsize`, ...)
- `--prefix`: path to the directory where the library will be installed
- `--default-library`: sets the type of library which is built (`static`, `shared` or `both`)

This library depends on [glfw](https://www.glfw.org),
[freetype](https://freetype.org/), [utfcpp](https://github.com/nemtrif/utfcpp)
and [glm](https://glm.g-truc.net/0.9.9/index.html). Except for glm which is
included in the sources, the other libraries are either found on the system or
dowloaded through Meson's WrapDB packages. In order to automatically download
the libraries, you should execute the command `meson wrap update-db` before
configuring the project. 

## Example

The installation procedure should look something like this:

```bash
git clone https://github.com/tretre91/MyGL.git
cd MyGL
meson wrap update-db
meson setup build --buildtype=release
cd build
meson compile
meson install
```

## Build the documentation

In order to build the documentation you will need to have [doxygen](https://www.doxygen.nl/download.html)
installed and enable the `mygl_build_docs` or `mygl_docs_only` option when configuring the project.

```bash
git clone https://github.com/tretre91/MyGL.git
cd MyGL
meson setup build -Dmygl_docs_only=true
cd build
meson compile docs/docs # only build the docs target
meson install
```
