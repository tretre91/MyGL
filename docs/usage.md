# How to use  {#usage}

## Compilation

This table sums up which libraries you have to link against on Windows and Linux :

|  Linking options |  Libraries to link against                                                                                                          ||
|------------------|------------------------------------------------------------------------------------------|-------------------------------------------|
|                  |  **On Windows**                                                                          |  **On Linux**                             |
|  Static linking  |  libmygl.a                                                                               |  libmygl.a                                |
|  Dynamic linking |  mygl.lib                                                                                |  libmygl.so                               |
|  In both         |  glfw3.lib, freetype.lib, opengl32.lib, gdi32.lib, user32.lib, shell32.lib, winmm.lib    |  libglfw3, libfreetype, libdl, libpthread |

### Windows

TODO

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

@note
If you wish to link with the static version of the library, you should define the `MYGL_STATIC` macro when compiling, like this:
```
g++ -c prog.cpp -DMYGL_STATIC
```
@note

## Usage

This library provides some abstraction over openGL to draw shapes and text, it
also provides a way to create windows and an event system.

The following example program displays a magenta rectangle on a light green background

```cpp
#include <MyGL/mygl.hpp>

int main() {
    my::Window window(800, 600, "my window!");
    window.setFramerate(60);

    my::Rectangle rect(70, 50, 400, 300);
    rect.setColor(155, 32, 104);

    my::Color clear_color(175, 230, 178);

    my::Event e;

    while(window.isRunning()) {
        while(window.pollEvent(e)){
            switch(e.type)
            {
            case my::EventType::windowShouldClose:
                window.close();
                break;
                
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

Then we create a 800 x 600 window with the title "my window!" and limit its 
framerate at 60 fps
```cpp
my::Window window(800, 600, "my window!");
window.setFramerate(60);
```

\warning You should create a window before using any of the library functions, as the library initialization happens in the window constructor


We create a rectangle using the Rectangle class, and set its color
```cpp
my::Rectangle rect(70, 50, 400, 300);
rect.setColor(155, 32, 104);
```

Then comes the main loop, while the window is opened:

1. We check if there are events that need to be processed :
   ```cpp
   while (window.pollEvent(e)) {
       switch (e.type)
       {
       case my::EventType::windowShouldClose:
           window.close();
           break;
           
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
   Here we process the `windowShouldClose` and `keyPressed` events.
   - The `windowShouldClose` event is triggered when the user tries to close the
   window, in our example if the user tries to close the window we effectively
   close it with a call to `window.close()`.
   - The `keyPressed` event is triggered when a keyboard key is pressed, here we
   close the window if the user presses the escape key.

2. Then we clear the window with a background color and draw our rectangle :
   ```cpp
   window.clear(clear_color);
   window.draw(rect);
   ```

3. Finally we display everything that has been drawn with :
   ```cpp
   window.display();
   ```
