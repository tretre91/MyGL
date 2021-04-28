#include <MyGL/mygl.hpp>

#include <iostream>

int main() {
    my::GLWindow window(800, 600, "Test", 2);
    window.setFramerate(75);

    my::Camera camera(0, 0);
    camera.setSpeed(100.0f);
    window.setCamera(camera);

    my::Rectangle rect(100, 50, 400, 300);
    rect.setColor(175, 87, 43, 255);

    unsigned int sides = 5;
    my::Polygon poly(sides, 100, 150, 300);
    poly.setColor(my::Color::white);


    my::Font openSans("@RESSOURCES_DIR@/Fonts/OpenSans-Regular.ttf");
    my::Text text("The .\\quick \"brown\" {fox} \n#jumps [over] the lazy dog!", openSans, 60);
    text.setPosition(5, 5);
    text.setOutlineThickness(5);
    text.setColor(my::Color::white);
    text.setOutlineColor(my::Color::red);

    const my::Color clearColor("1E1E1E");
    my::Event e;

    bool click = false;
    int mouseX = 0;
    int mouseY = 0;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;

    while (window.isRunning()) {
        while (window.pollEvent(e)) {
            switch (e.type)
            {
            case my::EventType::windowShouldClose:
                window.close();
                break;

            case my::EventType::keyPressed:
                switch (e.keyCode)
                {
                case my::Key::escape:
                    window.close();
                    break;

                case my::Key::left:
                    moveLeft = true;
                    break;

                case my::Key::right:
                    moveRight = true;
                    break;

                case my::Key::up:
                    moveUp = true;
                    break;

                case my::Key::down:
                    moveDown = true;
                    break;

                case my::Key::s:
                    const glm::ivec2 s = window.getSize();
                    window.setSize(s.x - 50, s.y - 50);
                    break;
                }
                break;

            case my::EventType::keyReleased:
                switch (e.keyCode)
                {
                case my::Key::left:
                    moveLeft = false;
                    break;

                case my::Key::right:
                    moveRight = false;
                    break;

                case my::Key::up:
                    moveUp = false;
                    break;

                case my::Key::down:
                    moveDown = false;
                    break;

                case my::Key::enter:
                    sides = sides > 10 ? 3 : sides + 1;
                    poly.setSides(sides);
                    break;

                default:
                    if (!e.mods.ctrl) click = false;
                }
                break;


            case my::EventType::mouseButtonPressed:
                if (e.mouse.button == my::MouseButton::left && e.mods.ctrl)
                    click = true;
                break;

            case my::EventType::mouseButtonReleased:
                if (e.mouse.button == my::MouseButton::left)
                    click = false;
                break;

            case my::EventType::mouseMoved:
                mouseX = e.mouse.pos.x;
                mouseY = e.mouse.pos.y;
                break;

            default:
                break;
            }
        }

        float frametime = static_cast<float>(window.getFrametime());
        if (moveLeft) camera.moveLeft(frametime);
        if (moveRight) camera.moveRight(frametime);
        if (moveUp) camera.moveUp(frametime);
        if (moveDown) camera.moveDown(frametime);

        if (click) rect.setRelativePosition(mouseX, mouseY, window.getCamera(), true);

        window.clear(clearColor);
        window.draw(rect);
        window.draw(poly);
        window.draw(text);
        window.display();
    }
}