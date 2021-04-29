#include <MyGL/mygl.hpp>

int main() {
    my::GLWindow window(800, 600, "Rectangle");
    window.setFramerate(60);

    my::Rectangle rectangle(200, 100);
    rectangle.setColor(my::Color::red);
    rectangle.setPosition(400, 300, true);

    my::Event event;
    while (window.isRunning()) {
        while (window.pollEvent(event)) {
            switch (event.type)
            {
            case my::EventType::windowShouldClose:
                window.close();
                break;

            case my::EventType::keyPressed:
                if (event.keyCode == my::Key::escape)
                    window.close();
                break;

            default:
                break;
            }
        }

        window.clear(my::Color::black);

        window.draw(rectangle);

        window.display();
    }
}