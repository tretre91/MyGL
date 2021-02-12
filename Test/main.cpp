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
    auto distance = [](int x1, int y1, int x2, int y2) {
        return glm::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    };
    double dist = distance(x1, y1, x2, y2);
    int center_x = (x2 + x1) / 2;
    int center_y = (y2 + y1) / 2;
    my::Rectangle res(glm::round(dist), 1);
    res.setPosition(center_x, center_y, true);
    double cos = (glm::abs(x2 - x1) / 2) / (dist / 2.0f);
    int angle = glm::round(glm::degrees(glm::acos(cos)));
    res.setRotation(angle);

    return res;
}

int main(int argc, char* argv[]) {
    // initialization
    my::GLWindow window(800, 600, "OpenGL!", 4);
    //window.setFramerateLimit(0);
    if (!gladLoadGLLoader(my::GLWindow::getGLProcAdress)) {
        std::cerr << "ERREUR" << std::endl;
        exit(EXIT_FAILURE);
    }
    /*******************************************************************************/
    window.enableVsync(false);
    my::Color test("29bc9c", 100);
    my::Color alertColor("812d2a", 128);
    my::Rectangle rectangle(50, 40);
    rectangle.setColor(test);
    bool moveByCenter = false;

    my::Rectangle meme(500, 350, 300, 600);
    meme.setColor(my::Color::blue);
    meme.setTexture("@RESSOURCES_DIR@/Images/meme.jpeg");

    my::Rectangle ligne = line(0, 0, 800, 600);
    ligne.setColor(my::Color::red);
    //ligne.setRotation(45);

    my::Rectangle smiley(100, 70, 650, 300);
    smiley.setTexture("@RESSOURCES_DIR@/Images/awesomeface.png", true);

    my::Polygon<8> wall(60, 100, 530);
    wall.setTexture("@RESSOURCES_DIR@/Images/wall.jpg");

    my::Polygon<5> blue(50, 0, 0);
    blue.setColor(my::Color::blue);

    my::Polygon<5> green(50, 20, 30);
    green.setColor(my::Color::green); 

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
    Uint32 tickCount = SDL_GetTicks();
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

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        window.clear(clearColor);

        frametime = (SDL_GetTicks() - tickCount) / 1000.0f;
        tickCount = SDL_GetTicks();

        
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
            if (rectangle.colides(&blue)) rectangle.setColor(alertColor);
            else rectangle.setColor(test);
            moved = false;
        }

        window.draw(meme);
        window.draw(smiley);
        window.draw(wall);
        window.draw(rectangle);
        window.draw(blue);
        window.draw(green);
        window.draw(text);
        window.draw(ligne);

        window.display();
        frameCount++;
    }

    return 0;
}