#include <MyGL/Window.hpp>
using namespace my;

bool GLWindow::gladIsInitialized = false;

GLWindow::GLWindow() : m_projection(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 10.0f)), m_camera(nullptr),
sf::Window(sf::VideoMode(800, 600), "Default", sf::Style::Default, sf::ContextSettings(24, 8, 2, 3, 3, sf::ContextSettings::Core)) {
	this->setActive(true);
	if (!gladIsInitialized) {
		if (!gladLoadGL()) {
			std::cout << "ERROR: FAILED TO INITIALIZE GLAD" << std::endl;
			exit(-1);
		}
		gladIsInitialized = true;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLWindow::GLWindow(int width, int height, const std::string& title) : m_projection(glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), 0.1f, 10.0f)),
m_camera(nullptr), sf::Window(sf::VideoMode(width, height), title, sf::Style::Default, sf::ContextSettings(24, 8, 2, 3, 3, sf::ContextSettings::Core)) {
	this->setActive(true);
	if (!gladIsInitialized) {
		if (!gladLoadGL()) {
			std::cout << "ERROR: FAILED TO INITIALIZE GLAD" << std::endl;
			exit(-1);
		}
		gladIsInitialized = true;
	}

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLWindow::GLWindow(sf::VideoMode mode, const std::string& title, sf::Uint32 style) : m_projection(glm::ortho(0.0f, static_cast<float>(mode.width), 0.0f, static_cast<float>(mode.height), 0.1f, 10.0f)),
m_camera(nullptr), sf::Window(mode, title, style, sf::ContextSettings(24, 8, 2, 3, 3, sf::ContextSettings::Core)) {
	this->setActive(true);
	if (!gladIsInitialized) {
		if (!gladLoadGL()) {
			std::cout << "ERROR: FAILED TO INITIALIZE GLAD" << std::endl;
			exit(-1);
		}
		gladIsInitialized = true;
	}

	glViewport(0, 0, mode.width, mode.height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLWindow::clear(const my::Color& color) const {
	glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::setCamera(my::FixedCamera& camera) {
	m_camera = &camera;
}

void GLWindow::setProjection(const glm::mat4& projection) {
	m_projection = projection;
}

void GLWindow::draw(my::AbstractShape& shape) {
	my::AbstractShape* ptr = &shape;
	ptr->draw(m_camera->lookAt(), m_projection);
}