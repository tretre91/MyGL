mygl_headers = files([
  'MyGL/Camera.hpp',
  'MyGL/Color.hpp',
  'MyGL/common.hpp',
  'MyGL/Cursor.hpp',
  'MyGL/Event.hpp',
  'MyGL/Image.hpp',
  'MyGL/mygl_export.h',
  'MyGL/mygl.hpp',
  'MyGL/Shader.hpp',
  'MyGL/ShaderProgram.hpp',
  'MyGL/Texture.hpp',
  'MyGL/Window.hpp',
  'MyGL/Drawable/AbstractShape.hpp',
  'MyGL/Drawable/Font.hpp',
  'MyGL/Drawable/Polygon.hpp',
  'MyGL/Drawable/Rectangle.hpp',
  'MyGL/Drawable/Text.hpp',
  'MyGL/Drawable/Triangle.hpp'
])

if not get_option('mygl_docs_only')
  install_headers(mygl_headers, 'glad/gl.h', 'stb_image.h',
    preserve_path : true,
    subdir : 'MyGL'
  )

  install_subdir('glm', install_dir : 'include/MyGL')
endif
