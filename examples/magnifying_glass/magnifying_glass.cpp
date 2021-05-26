#include <MyGL/mygl.hpp>

int main() {
    my::GLWindow window(800, 600, "Magnifying glass", 0);
    window.setFramerate(60);

    my::Camera camera(0, 0);
    window.setCamera(camera);

    my::Camera frameCam(0, 0);

    my::Rectangle rect(800, 600);
    rect.setPosition(0, 0);
    rect.setColor(my::Color("D57E4D"));
    rect.setTexture("@RESSOURCES_DIR@/galaxy.jpg");

    const int frameSize = 80;
    my::Rectangle frame(frameSize, frameSize);
    frame.setColor(0, 0, 0, 0);
    frame.setOutlineThickness(3);
    frame.setOutlineColor(my::Color::red);

    glm::ivec2 framePos(0, 0);
    int zoomValue = 0;

    my::Event e;
    while (window.isRunning()) {
        while (window.pollEvent(e)) {
            switch (e.type) {
            case my::EventType::windowShouldClose:
                window.close();
                break;

            case my::EventType::keyPressed:
                if (e.keyCode == my::Key::escape)
                    window.close();
                break;

            case my::EventType::mouseMoved:
                framePos = e.mouse.pos - frame.getSize() / 2.0f;
                frame.setRelativePosition(framePos, camera);
                frameCam.setPosition(framePos.x, framePos.y);
                break;

            case my::EventType::mouseScrolled:
                zoomValue = glm::clamp(zoomValue + 3 * e.scrollOffset.y, 0, (frameSize / 2) - 1);
                break;

            default:
                break;
            }
        }

        window.clear(my::Color::black);

        window.setCamera(camera);
        window.setClipPlanes(0, window.getSize().x, 0, window.getSize().y);
        window.setViewport(0, 0, window.getSize().x, window.getSize().y);

        window.draw(rect);
        window.draw(frame);

        window.setCamera(frameCam);
        window.setClipPlanes(zoomValue, frameSize - zoomValue, zoomValue, frameSize - zoomValue);
        window.setViewport(framePos.x, framePos.y, frameSize, frameSize);

        window.draw(rect);

        window.display();
    }

    return 0;
}
