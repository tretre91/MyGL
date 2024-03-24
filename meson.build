project('MyGL', 'c', 'cpp',
        license: 'zlib',
        version: '0.1.0',
        default_options: ['cpp_std=c++17'])


subdir('src')
subdir('include')

if get_option('mygl_build_examples')
  subdir('examples')
endif

if get_option('mygl_build_doc')
  subdir('docs')
endif

# installation
pkgc = import('pkgconfig')
pkgc.generate(
  mygl,
  name: 'mygl',
  version: meson.project_version(),
  description: 'A little graphics library',
  url: 'https://github.com/tretre91/MyGL',
  subdirs: ['MyGL'],
  requires: ['glfw3', 'freetype2']
)

