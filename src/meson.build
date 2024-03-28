mygl_sources = files([
  'glad.c',
  'Color.cpp',
  'Shader.cpp',
  'ShaderProgram.cpp',
  'stb_image.cpp',
  'Texture.cpp',
  'Window.cpp',
  'Drawable/AbstractShape.cpp',
  'Drawable/Rectangle.cpp',
  'Drawable/Polygon.cpp',
  'Drawable/Triangle.cpp',
  'Drawable/Font.cpp',
  'Drawable/Text.cpp',
  'Camera.cpp',
  'Event.cpp',
  'Cursor.cpp',
  'common.cpp',
  'Image.cpp',
])

# dependencies
glfw = dependency('glfw3', default_options: { 'install': true,})
freetype = dependency('freetype2')
utf = dependency('utf8cpp')
# find winmm (windows only)
cxx = meson.get_compiler('cpp')
winmm = cxx.find_library('winmm', required: false)

mygl_deps = [glfw, freetype, utf, winmm]

include_dir = '..' / 'include'

mygl = library('mygl', mygl_sources,
        cpp_args : ['-DUTF_CPP_CPLUSPLUS=201703L'],
        c_static_args : ['-DMYGL_STATIC'],
        cpp_static_args : ['-DMYGL_STATIC'],
        c_shared_args : ['-DMYGL_BUILD'],
        cpp_shared_args : ['-DMYGL_BUILD'],
        include_directories: include_dir,
        dependencies: mygl_deps,
        gnu_symbol_visibility: 'inlineshidden',
        install: true)

mygl_dep = declare_dependency(
  link_with: mygl,
  dependencies: mygl_deps,
  include_directories: include_dir
)
