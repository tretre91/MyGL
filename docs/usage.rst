.. Usage indications for linking and using the library

Usage
=====

When compiling
--------------

On Windows
..........

Dynamic linking
+++++++++++++++

When using the dll version of the library, you should link against :

* mygl.lib (or mygld.lib for a debug configuration)
* SDL2.lib (or SDL2d.lib for debug)
* SDL2main.lib
* freetype.lib (freetyped.lib for debug)

and place the files mygl.dll and SDL2.dll (respectively mygld.dll and SDL2d.dll for debug configs)
in the same folder as your executable

Static linking
++++++++++++++

For the static version :

* mygl-static.lib (or mygl-staticd.lib for a debug configuration)
* SDL2-static.lib (or SDL2-staticd.lib for debug)
* SDL2main.lib
* freetype.lib (freetyped.lib for debug)

you should also link SDL's dependencies : user32, gdi32, winspool, comdlg32,
advapi32, shell32, ole32, oleaut32, uuid, odbc32, odbccp32

On Linux
........

On Linux you will need to link against :

* mygl (or mygld)
* freetype
* SDL2
* SDL2main

Remark : if you happen to link against the static binary (libmygl(d).a) you should 
also link against libdl

The compilation process for a simple program should look something like this :

.. code-block::

	$ g++ -c prog.cpp
	$ g++ prog.o -o prog -lmygl -lSDL2 -lSDLmain -lfreetype

Using the library
-----------------

This library provides some abstraction over openGL to draw shapes and text, it
uses SDL2 for the underlying implementation of windows and for the event system

.. code-block:: cpp
	:linenos:
	
	#include <MyGL/Camera.hpp>
	#include <MyGL/Drawable.hpp>
	#include <MyGL/Window.hpp>

	int main(int argc, char *argv[]) {
	  my::GLWindow window(800, 600, "my window!");
	
	  my::Cam2D camera(0, 0);
	  window.setCamera(camera);

	  my::Rectangle rect(70, 50, 400, 300);
	  rect.setColor(155, 32, 104);

	  my::Color clear_color(175, 230, 178);

	  bool running = true;
	  SDL_Event event;

	  while(running) {
	    while(SDL_PollEvent(&event)){
	      switch(event.type)
	      {
	      case SDL_QUIT:
	        running = false;
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