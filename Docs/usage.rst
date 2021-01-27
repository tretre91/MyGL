.. Usage indications for linking and using the library

Usage
=====

When compiling
--------------

Because this library uses relies on SFML's window and system modules, you will need
(at least on Linux) to have its dependencies installed in order to successfuly build
your program.

The necessary dependencies are :

* GL
* pthread
* X11 
* Xrandr
* udev

Windows
.......

On Windows, only linking against the library mygl.lib (or mygld.lib for debug 
configs) *should* work

Linux
.....

On Linux you have to link the library but also it's dependancies library files.
The necessary packages are the development packages of these libs, for example
on Ubuntu the corresponding packages are :

* libgl1-mesa-dev
* libpthread-stubs0-dev
* libx11-dev
* libxrandr-dev
* libudev-dev


Using the library
-----------------

This library provides some abstraction over openGL to use blablabla

.. code-block:: cpp
	:linenos:
	
	#include <MyGL/Camera.hpp>
	#include <MyGL/Drawable.hpp>
	#include <MyGL/Window.hpp>

	int main() {
	  my::GLWindow window(800, 600, "my window!");
	
	  my::Cam2D camera(0, 0);
	  window.setCamera(camera);

	  my::Rectangle rect(70, 50, 400, 300);
	  rect.setColor(155, 32, 104);

	  my::Color clear_color(175, 230, 178);

	  bool running = true;

	  while(running) {
	    sf::Event event;
	    while(window.pollEvent(event)){
	      switch(event.type)
	      {
	      case sf::Event::Closed:
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