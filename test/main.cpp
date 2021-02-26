#include <MyGL/Camera.hpp>
#include <MyGL/Window.hpp>
#include <MyGL/Drawable.hpp>

#include <iostream>

Uint32 framerateCallback(Uint32 interval, void* param) {
    int* frameCount = reinterpret_cast<int*>(param);
    std::cout << *frameCount << '\n';
    *frameCount = 0;
    return interval;
}

my::Rectangle line(int x1, int y1, int x2, int y2) {
    double dist = glm::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    int center_x = (x2 + x1) / 2;
    int center_y = (y2 + y1) / 2;
    my::Rectangle res(std::lround(dist), 1);
    res.setPosition(center_x, center_y, true);
    double cos = (glm::abs(x2 - x1) / 2) / (dist / 2.0f);
    int angle = glm::round(glm::degrees(glm::acos(cos)));
    res.setRotation(angle);

    return res;
}

int main(int argc, char* argv[]) {
    // initialization
    my::GLWindow window(800, 600, "OpenGL!", 8);
    //window.setFramerate(0);
    /*******************************************************************************/
    window.enableVsync(false);
    my::Color test("29bc9c", 100);
    my::Color alertColor("812d2a", 128);
    my::Rectangle rectangle(50, 40);
    rectangle.setColor(test);
    bool moveByCenter = false;

    my::Rectangle meme(500, 350, 550, 425);
    meme.setColor(my::Color::blue);
    meme.setTexture("@RESSOURCES_DIR@/Images/meme.jpeg");

    my::Rectangle ligne = line(0, 0, 800, 600);
    ligne.setColor(my::Color::white);
    ligne.setOutlineThickness(5);
    ligne.setOutlineColor(0, 255, 255);

    my::Rectangle smiley(100, 70, 650, 300);
    smiley.setTexture("@RESSOURCES_DIR@/Images/awesomeface.png", true);

    my::Polygon<8> wall(60, 100, 530);
    wall.setTexture("@RESSOURCES_DIR@/Images/wall.jpg");
    wall.setOutlineThickness(5);
    wall.setOutlineColor(my::Color::red);

    my::Polygon<20> blue(50, 0, 0);
    blue.setColor(my::Color::blue);

    my::Polygon<5> green(50, 20, 30);
    green.setColor(my::Color::green); 

    my::Animation anim(green, 5.0f, 600.0f, 300.0f);

    my::Font arial("@RESSOURCES_DIR@/Fonts/OpenSans-Regular.ttf");
    const unsigned int size = 60;
    my::Text text("The .\\quick \"brown\" {fox} \n#jumps [over] the lazy dog!", arial, size);
    text.setColor(my::Color::blue);
    text.setPosition(0, 60);

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    bool rotateLeft = false;
    bool rotateRight = false;
    bool moved = false;

    const float pi = 3.141592f;
    float angle = 0.0f;
    float inc = pi / 2.0f;

    rectangle.setPosition(400 + (int)(100 * glm::cos(angle)), 300 + (int)(100 * glm::sin(angle)));
    blue.setPosition(400 + (int)(-100 * glm::cos(angle)), 300 + (int)(-100 * glm::sin(angle)));

    bool wireframe = false;
    bool running = true;
    my::Color clearColor(51, 72, 93);

    my::Cam2D camera(0, 0);
    camera.setSpeed(100.0f);
    window.setCamera(camera);

    int frameCount = 0;
    //std::iostream::sync_with_stdio(false);

    SDL_TimerID framerateTimer = SDL_AddTimer(1000, framerateCallback, (void*)&frameCount);
    float frametime = 0.0f;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;

                case SDLK_z:
                    up = true;
                    break;

                case SDLK_s:
                    down = true;
                    break;

                case SDLK_q:
                    left = true;
                    break;

                case SDLK_d:
                    right = true;
                    break;

                case SDLK_LEFT:
                    rotateLeft = true;
                    break;

                case SDLK_RIGHT:
                    rotateRight = true;
                    break;

                default:
                    break;
                }
                break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_RETURN:
                    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    wireframe = !wireframe;
                    break;

                case SDLK_z:
                    up = false;
                    break;

                case SDLK_s:
                    down = false;
                    break;

                case SDLK_q:
                    left = false;
                    break;

                case SDLK_d:
                    right = false;
                    break;

                case SDLK_LEFT:
                    rotateLeft = false;
                    break;

                case SDLK_RIGHT:
                    rotateRight = false;
                    break;

                case SDLK_a:
                    anim.setTargetPosition(600.0f, 300.0f);
                    if (anim.isOver()) anim.start();
                    else anim.setDuration(2.5f);
                    break;

                case SDLK_e:
                    anim.setTargetPosition(50.0f, 550.0f);
                    if (anim.isOver()) anim.start();
                    else anim.setDuration(2.5);
                    break;

                case SDLK_r:
                    anim.reset();
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        window.clear(clearColor);

        frametime = window.getFrametime();

        
        if (up) camera.moveUp(frametime);
        if (down) camera.moveDown(frametime);
        if (left) camera.moveLeft(frametime);
        if (right) camera.moveRight(frametime);

        if (rotateLeft) {
            angle += inc * frametime;
            if (angle > 2 * pi) angle = 0.0f;
            moved = true;
        }
        if (rotateRight) {
            angle -= inc * frametime;
            if (angle < 0.0f) angle = 2 * pi;
            moved = true;
        }

        if (moved) {
            //rectangle.setPosition(400 + (int)(100 * glm::cos(angle)), 300 + (int)(100 * glm::sin(angle)));
            blue.setPosition(400 + (int)(-100 * glm::cos(angle)), 300 + (int)(-100 * glm::sin(angle)));
            if (rectangle.BBoxCollides(blue)) rectangle.setColor(alertColor);
            else rectangle.setColor(test);
            if (blue.SATCollides(ligne)) blue.setColor(alertColor);
            else blue.setColor(my::Color::blue);
            moved = false;
        }

        window.draw(meme);
        window.draw(smiley);
        window.draw(wall);
        window.draw(rectangle);
        //window.draw(green);
        window.draw(anim);
        window.draw(ligne);
        window.draw(text);
        window.draw(blue);

        window.display();
        frameCount++;
    }

    return 0;
}