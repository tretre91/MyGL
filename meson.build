project('MyGL', 'c', 'cpp',
        license: 'zlib',
        version: '0.1.0',
        default_options: ['cpp_std=c++20'])

subdir('include')

if not get_option('mygl_docs_only')
  subdir('src')

  if get_option('mygl_build_examples')
    subdir('examples')
  endif

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
endif

if get_option('mygl_build_docs') or get_option('mygl_docs_only')
  subdir('docs')
endif
