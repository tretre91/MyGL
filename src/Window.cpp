#include <MyGL/Window.hpp>
using namespace my;

bool GLWindow::gladIsInitialized = false;
unsigned int GLWindow::instancesCount = 0;

GLWindow::GLWindow() : GLWindow(800, 600, "Default") {}

GLWindow::GLWindow(int width, int height, const std::string& title, unsigned short aa) : 
m_projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.1f, 10.0f)),
p_camera(nullptr), p_window(nullptr), m_glContext(), m_frametime(1.0f), m_tickCount(0), m_frameDelay(0)
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
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
    
    if(aa) {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        if(aa < 2) aa = 1;
        else if(aa < 4) aa = 2;
        else if(aa < 8) aa = 4;
        else aa = 8;
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, aa);
    }

    p_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (p_window == nullptr) {
        std::cerr << "ERROR::SDL: Failed to create a window" << std::endl;
        exit(EXIT_FAILURE);
    }

    m_glContext = SDL_GL_CreateContext(p_window);

    if (!gladIsInitialized) {
        //SDL_SetMainReady();
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) || !initGlad()) {
            std::cout << "ERROR::GLAD: Failed to initialize GLAD" << std::endl;
            SDL_GL_DeleteContext(m_glContext);
            SDL_DestroyWindow(p_window);
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
        gladIsInitialized = true;
    }

    SDL_GL_SetSwapInterval(0);
    m_tickCount = SDL_GetTicks();

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLWindow::~GLWindow() {
    instancesCount--;
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(p_window);
    if (instancesCount == 0) {
        SDL_Quit();
    }
}

bool GLWindow::setActive(bool activate) {
    if (SDL_GL_MakeCurrent(p_window, m_glContext)) {
        // TODO
        return false;
    }
    return true;
}

void GLWindow::setFramerate(unsigned int limit) {
    m_frameDelay = limit == 0 ? 0 : 1000 / limit;
}

void GLWindow::enableVsync(bool enable) {
    if(enable) {
        if(SDL_GL_SetSwapInterval(-1) == -1) SDL_GL_SetSwapInterval(1);
        m_frameDelay = 0;
    } else {
        SDL_GL_SetSwapInterval(0);
    }
}

void GLWindow::clear(const my::Color& color) const {
    glm::vec4 nColor(color.getNormalized());
    glClearColor(nColor.r, nColor.g, nColor.b, nColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLWindow::setCamera(my::FixedCamera& camera) {
    p_camera = &camera;
}

void GLWindow::setProjection(const glm::mat4& projection) {
    m_projection = projection;
}

void GLWindow::draw(my::AbstractShape& shape) {
    my::AbstractShape* ptr = &shape;
    ptr->draw(p_camera->lookAt(), m_projection);
}

void GLWindow::display() const {
    SDL_GL_SwapWindow(p_window);
    Uint32 tmp = SDL_GetTicks() - m_tickCount;
    if(tmp < m_frameDelay) SDL_Delay(m_frameDelay - tmp);
    tmp = SDL_GetTicks();
    m_frametime = (tmp - m_tickCount) / 1000.0f;
    m_tickCount = tmp;
}

float GLWindow::getFrametime() const {
    return m_frametime;
}

void* GLWindow::getGLProcAdress(const char* name) {
    return (void*)SDL_GL_GetProcAddress(name);
}