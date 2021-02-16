#include <MyGL/Window.hpp>
using namespace my;

bool GLWindow::gladIsInitialized = false;
unsigned int GLWindow::instancesCount = 0;

GLWindow::GLWindow() : GLWindow(800, 600, "Default") {}

GLWindow::GLWindow(int width, int height, const std::string& title, unsigned short aa) : mProjection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.1f, 10.0f)),
pCamera(nullptr), pWindow(nullptr), mGLContext(), mFrametime(1.0f), mTickCount(0), mFrameDelay(0)
{
    if (instancesCount == 0) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            std::cerr << "ERROR::SDL2: Failed to initialize Video module" << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_GL_LoadLibrary(NULL);
    }
    instancesCount++;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    if(aa) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        if(aa < 2) aa = 1;
        else if(aa < 4) aa = 2;
        else if(aa < 8) aa = 4;
        else aa = 8;
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, aa);
    }

    pWindow = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (pWindow == nullptr) {
        std::cerr << "ERROR::SDL: Failed to create a window" << std::endl;
        exit(EXIT_FAILURE);
    }

    mGLContext = SDL_GL_CreateContext(pWindow);

    if (!gladIsInitialized) {
        //SDL_SetMainReady();
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) || !initGlad()) {
            std::cout << "ERROR::GLAD: Failed to initialize GLAD" << std::endl;
            SDL_GL_DeleteContext(mGLContext);
            SDL_DestroyWindow(pWindow);
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
        gladIsInitialized = true;
    }

    SDL_GL_SetSwapInterval(0);
    mTickCount = SDL_GetTicks();

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLWindow::~GLWindow() {
    instancesCount--;
    SDL_GL_DeleteContext(mGLContext);
    SDL_DestroyWindow(pWindow);
    if (instancesCount == 0) {
        SDL_Quit();
    }
}

bool GLWindow::setActive(bool activate) {
    if (SDL_GL_MakeCurrent(pWindow, mGLContext)) {
        // TODO
        return false;
    }
    return true;
}

void GLWindow::setFramerate(unsigned int limit) {
    mFrameDelay = limit == 0 ? 0 : 1000 / limit;
}

void GLWindow::enableVsync(bool enable) {
    if(enable) {
        if(SDL_GL_SetSwapInterval(-1) == -1) SDL_GL_SetSwapInterval(1);
        mFrameDelay = 0;
    } else {
        SDL_GL_SetSwapInterval(0);
    }
}

void GLWindow::clear(const my::Color& color) const {
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::setCamera(my::FixedCamera& camera) {
    pCamera = &camera;
}

void GLWindow::setProjection(const glm::mat4& projection) {
    mProjection = projection;
}

void GLWindow::draw(my::AbstractShape& shape) {
    my::AbstractShape* ptr = &shape;
    ptr->draw(pCamera->lookAt(), mProjection);
}

void GLWindow::display() const {
    SDL_GL_SwapWindow(pWindow);
    Uint32 tmp = SDL_GetTicks() - mTickCount;
    if(tmp < mFrameDelay) SDL_Delay(mFrameDelay - tmp);
    tmp = SDL_GetTicks();
    mFrametime = (tmp - mTickCount) / 1000.0f;
    mTickCount = tmp;
}

float GLWindow::getFrametime() const {
    return mFrametime;
}

void* GLWindow::getGLProcAdress(const char* name) {
    return (void*)SDL_GL_GetProcAddress(name);
}