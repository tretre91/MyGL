#include <MyGL/mygl.hpp>

int main() {
    my::GLWindow window(800, 600, "Test", 0);
    window.setFramerate(10);

    my::Camera cam(0, 0);
    window.setCamera(cam);

    my::Polygon pentagon(5, 50, 400, 300);
    pentagon.setColor(152, 102, 206);
    pentagon.setOutlineThickness(5);
    pentagon.setOutlineColor(my::Color::white);

    my::Color clearColor("1E1E1E");
    int countdown = 25;

    while (window.isRunning()) {
        if (countdown-- == 0) {
            window.close();
        }
        window.clear(clearColor);
        window.draw(pentagon);
        window.display();
    }
}
