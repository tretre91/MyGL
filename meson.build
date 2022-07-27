project('MyGL', 'c', 'cpp',
        license: 'zlib',
        version: '0.1.0',
        default_options: ['cpp_std=c++17'])

mygl_sources = [
  'src/glad.c',
  'src/Color.cpp',
  'src/Shader.cpp',
  'src/ShaderProgram.cpp',
  'src/stb_image.cpp',
  'src/Texture.cpp',
  'src/Window.cpp',
  'src/Drawable/AbstractShape.cpp',
  'src/Drawable/Rectangle.cpp',
  'src/Drawable/Polygon.cpp',
  'src/Drawable/Triangle.cpp',
  'src/Drawable/Font.cpp',
  'src/Drawable/Text.cpp',
  'src/Camera.cpp',
  'src/Event.cpp',
  'src/Cursor.cpp',
  'src/common.cpp',
  'src/Image.cpp',
  'include/MyGL/Camera.hpp',
  'include/MyGL/Color.hpp',
  'include/MyGL/common.hpp',
  'include/MyGL/Cursor.hpp',
  'include/MyGL/Event.hpp',
  'include/MyGL/Image.hpp',
  'include/MyGL/mygl_export.h',
  'include/MyGL/mygl.hpp',
  'include/MyGL/Shader.hpp',
  'include/MyGL/ShaderProgram.hpp',
  'include/MyGL/Texture.hpp',
  'include/MyGL/Window.hpp',
  'include/MyGL/Drawable/AbstractShape.hpp',
  'include/MyGL/Drawable/Font.hpp',
  'include/MyGL/Drawable/Polygon.hpp',
  'include/MyGL/Drawable/Rectangle.hpp',
  'include/MyGL/Drawable/Text.hpp',
  'include/MyGL/Drawable/Triangle.hpp'
]

# dependencies
glfw_dep = dependency('glfw3')
freetype_dep = dependency('freetype2')
utf_dep = dependency('utfcpp')
# find winmm (windows only)
cxx = meson.get_compiler('cpp')
winmm_dep = cxx.find_library('winmm', required: false)

mygl_deps = [glfw_dep, freetype_dep, utf_dep, winmm_dep]

build_type = get_option('default_library')
compile_defs = ['-DUTF_CPP_CPLUSPLUS=201703L']
if build_type == 'shared'
  compile_defs += ['-DGLAD_GLAPI_EXPORT', '-DGLAD_GLAPI_EXPORT_BUILD']
elif build_type == 'static'
  compile_defs += ['-DMYGL_STATIC_DEFINE']
endif

mygl = library('mygl', mygl_sources,
        include_directories: ['include/'],
        dependencies: mygl_deps,
        c_args: compile_defs,
        cpp_args: compile_defs,
        gnu_symbol_visibility: 'inlineshidden',
        install: true)

mygl_dependency = declare_dependency(
  compile_args: compile_defs,
  link_with: mygl,
  sources: mygl_sources,
  dependencies: mygl_deps,
  include_directories: 'include/'
)

# installation
if not meson.is_subproject()
  # install all the headers
  install_subdir('include', install_dir: 'include/MyGL', strip_directory: true)
  
  pkgc = import('pkgconfig')
  pkgc.generate(
    mygl,
    name: 'mygl',
    version: meson.project_version(),
    description: 'A little graphics library',
    url: 'https://github.com/tretre91/MyGL',
    subdirs: 'MyGL',
    requires: ['glfw3', 'freetype2']
  )
endif

