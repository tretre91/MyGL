doxygen = find_program('doxygen')

markdown_sources = [
  meson.current_source_dir() / 'index.md',
  meson.current_source_dir() / 'install.md',
  meson.current_source_dir() / 'usage.md'
]

da_dir = meson.current_source_dir() / 'doxygen-awesome'
stylesheets = [da_dir / 'doxygen-awesome.css', da_dir / 'custom-style.css']
html_header = da_dir / 'header.html'
html_footer = da_dir / 'footer.html'
extra_files = [
  da_dir / 'doxygen-awesome-darkmode-toggle.js',
  da_dir / 'footer.js',
  da_dir / 'img' / 'dark_mode_black_24dp.svg',
  da_dir / 'img' / 'light_mode_white_24dp.svg',
]

input_dir = meson.source_root() / 'include' / 'MyGL'
output_dir = meson.current_build_dir()

doxyfile_config = configuration_data({
  'MARKDOWN_SOURCES': ' '.join(markdown_sources),
  'HTML_HEADER': html_header,
  'HTML_FOOTER': html_footer,
  'CUSTOM_CSS': ' '.join(stylesheets),
  'EXTRA_FILES': ' '.join(extra_files),
})
doxyfile_config.set_quoted('INPUT_DIR', input_dir)
doxyfile_config.set_quoted('OUTPUT_DIR', output_dir)

doxyfile = configure_file(
  input : 'Doxyfile.in',
  output : 'Doxyfile',
  configuration : doxyfile_config,
)

docs = custom_target('docs',
  input : [
    doxyfile,
    markdown_sources,
    html_header,
    html_footer,
    stylesheets,
    mygl_headers,
  ],
  output : 'html',
  install : true,
  install_dir : get_option('datadir') / 'MyGL',
  command : [doxygen, doxyfile]
)

