resources = ['galaxy.jpg']

foreach resource : resources
  fs.copyfile(resource)
  install_data(resource, install_dir : install_dir / 'resources')
endforeach
