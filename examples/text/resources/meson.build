resources = [
  'latinmodern-math_GUST-FONT-LICENSE.txt',
  'latinmodern-math.otf',
  'NotoSansJP-Regular.otf',
  'OFL.txt',
  'OpenSans_LICENSE.txt',
  'OpenSans-Regular.ttf',
]

foreach resource : resources
  fs.copyfile(resource)
  install_data(resource, install_dir : install_dir / 'resources')
endforeach
