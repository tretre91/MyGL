.. Usage indications for compiling, linking and using the library

Usage
=====

When compiling
--------------

This table sums up which libraries you have to link against on Windows and Linux :

+-----------------+--------------------------------------+
| Linking options | Libraries to link against            |
|                 +----------------------+---------------+
|                 | On Windows           | On Linux      |
+=================+======================+===============+
| Static linking  | * mygl-static(d).lib | * libmygl(d)  |
+-----------------+----------------------+---------------+
| Dynamic linking | * mygl(d).lib        | * libmygl(d)  |
+-----------------+----------------------+---------------+
| In both         | * glfw3.lib          | * libglfw3    |
|                 | * freetype(d).lib    | * libfreetype |
|                 | * opengl32.lib       | * libdl       |
|                 | * gdi32.lib          | * libpthread  |
|                 | * user32.lib         |               |
|                 | * shell32.lib        |               |
+-----------------+----------------------+---------------+

The libraries have the 'd' suffix for their debug configurations

.. note::
   When using the shared binaries on Windows, you should put the mygl(d).dll file in
   the same folder as your executable

Windows
```````
Visual Studio
:::::::::::::

You should add the include and lib folder of MyGL, GLFW and Freetype to the include 
directories and library directories in Visual studio. These are located in your 
project's properties : 

* C/C++ -> General -> Additional Include Directories
* Linker -> General -> Additional Library Directories

Then you should add the required libraries (shown in the table `here <When compiling>`_) in :

* Linker -> Input -> Additional Dependencies

If you use the static library, you should also add the macro ``MYGL_STATIC_DEFINE`` in :

* C/C++ -> Preprocessor -> Preprocessor Definitions

Linux
`````

The compilation process for a simple program should look something like this :

.. code-block:: bash

    $ g++ -c prog.cpp
    $ g++ prog.o -o prog -lmygl -lglfw3 -lfreetype -ldl -lpthread

This is assuming that the library was installed in a default location, otherwise it would be :

.. code-block:: bash
    
    $ g++ -c prog.cpp -I/path/to/include/folder/ -I/path/to/freetype/include/folder/ -I/path/to/glfw/include/folder/
    $ g++ prog.o -o prog -lmygl -lglfw3 -lfreetype -ldl -lpthread -L/path/to/lib/folder/


Using the library
-----------------

This library provides some abstraction over openGL to draw shapes and text, it
also provides a window and event system (both based on GLFW)

.. code-block:: cpp
    :linenos:
    
    #include <MyGL/mygl.hpp>

    int main() {
      my::GLWindow window(800, 600, "my window!");
      window.setFramerate(60);
    
      my::Cam2D camera(0, 0);
      window.setCamera(camera);

      my::Rectangle rect(70, 50, 400, 300);
      rect.setColor(155, 32, 104);

      my::Color clear_color(175, 230, 178);

      my::Event e;

      while(window.isRunning()) {
        while(window.pollEvent(e)){
          switch(e.type)
          {
          case my::EventType::keyPressed:
            if(e.keyCode == my::Key::escape) {
              window.close();
            }
            break;
          
          default:
            break;
          }
        }

        window.clear(clear_color);

        window.draw(rect);

        window.display();
      }

      return 0;
    }