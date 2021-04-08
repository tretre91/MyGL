# How to use

## Compilation

This table sums up which libraries you have to link against on Windows and Linux :

|  Linking options |  Libraries to link against                                                                                               ||
|------------------|-------------------------------------------------------------------------------|-------------------------------------------|
|                  |  **On Windows**                                                               |  **On Linux**                             |
|  Static linking  |  mygl-static(d).lib                                                           |  libmygl(d)                               |
|  Dynamic linking |  mygl(d).lib                                                                  |  libmygl(d)                               |
|  In both         |  glfw3.lib, freetype(d).lib, opengl32.lib, gdi32.lib, user32.lib, shell32.lib |  libglfw3, libfreetype, libdl, libpthread |

The libraries have the 'd' suffix for their debug configurations

### Windows

#### With Visual Studio

You should add the include and lib folder of MyGL, GLFW and Freetype to the
include directories and library directories in Visual studio. These are located
in your project's properties : 

* C/C++ -> General -> Additional Include Directories
* Linker -> General -> Additional Library Directories

Then you should add the required libraries (shown in [the table](#Compilation) above) in :

* Linker -> Input -> Additional Dependencies

If you use the static library, you should also add the macro `MYGL_STATIC_DEFINE` 
in :

* C/C++ -> Preprocessor -> Preprocessor Definitions

### Linux

The compilation process for a simple program should look something like this :

```bash
$ g++ -c prog.cpp
$ g++ prog.o -o prog -lmygl -lglfw3 -lfreetype -ldl -lpthread
```

This is assuming that the library was installed in a standard location, otherwise
it would be :

```bash
$ g++ -c prog.cpp -I/path/to/include/folder/ -I/path/to/freetype/include/folder/ -I/path/to/glfw/include/folder/
$ g++ prog.o -o prog -lmygl -lglfw3 -lfreetype -ldl -lpthread -L/path/to/lib/folder/
```

## Usage

This library provides some abstraction over openGL to draw shapes and text, it
also provides a window and event system.

The following program displays a magenta rectangle on a light green background

```cpp
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
```

We first include the library
```cpp
#include <MyGL/mygl.hpp>
```

Then we create a 800 x 600 window with the "my window!" title and set its 
framerate limit at 60 fps
```cpp
my::GLWindow window(800, 600, "my window!");
window.setFramerate(60);
```

\warning You should create a window before using any of the library functions, as the library initialization happens in the window constructor


We create a rectangle using the Rectangle class, and set its color
```cpp
my::Rectangle rect(70, 50, 400, 300);
rect.setColor(155, 32, 104);
```

Then comes the game loop, while the window is running:

1. We check if there are events that need to be processed :
   ```cpp
   while (window.pollEvent(e)) {
       switch (e.type)
       {
       case my::EventType::keyPressed:
           if (e.keyCode == my::Key::escape) {
               window.close();
           }
           break;
        
       default:
           break;
       }
   }
   ```
   Here we check if a key has been pressed, and if it was the escape key we 
   close the window

2. Then we clear the window with a background color and draw our rectangle :
   ```cpp
   window.clear(clear_color);
   window.draw(rect);
   ```

3. Finally we display everything that has been drawn with
   ```cpp
   window.display();
   ```
