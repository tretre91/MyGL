#include <MyGL/Camera.hpp>
#include <MyGL/Window.hpp>
#include <MyGL/Drawable.hpp>

#include <iostream>

int main() {
	// initialization
	my::GLWindow window(800, 600, "OpenGL!");
	window.setFramerateLimit(0);
	/*******************************************************************************/
	my::Color test("29bc9c", 100);
	my::Rectangle red(50, 40);
	//red.setColor(my::Color::red);
	red.setColor(test);
	red.positionByCenter(false);

	my::Polygon<5> blue(50, 0, 0);
	blue.setColor(my::Color::blue);

	my::Polygon<5> green(50, 20, 30);
	green.setColor(my::Color::green);

	my::Font arial("Ressources/Fonts/arial.ttf");
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

	red.setPosition(400 + (int)(100 * glm::cos(angle)), 300 + (int)(100 * glm::sin(angle)));
	blue.setPosition(400 + (int)(-100 * glm::cos(angle)), 300 + (int)(-100 * glm::sin(angle)));

	bool wireframe = false;
	bool running = true;
	my::Color clearColor(51, 72, 93);

	my::Cam2D camera(0, 0);
	camera.setSpeed(100.0f);
	window.setCamera(camera);

	int frameCount = 0;
	std::iostream::sync_with_stdio(false);

	sf::Clock frameClock;
	float second = 0.0f;
	float frametime;
	while (running) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::Resized:
				glViewport(0, 0, event.size.width, event.size.height);
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					running = false;
					break;
				case sf::Keyboard::Enter:
					if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					wireframe = !wireframe;
					break;
				case sf::Keyboard::Z:
					up = true;
					break;
				case sf::Keyboard::S:
					down = true;
					break;
				case sf::Keyboard::Q:
					left = true;
					break;
				case sf::Keyboard::D:
					right = true;
					break;
				case sf::Keyboard::Left:
					rotateLeft = true;
					break;
				case sf::Keyboard::Right:
					rotateRight = true;
					break;
				case sf::Keyboard::Up:
					red.move(0, 1);
					break;
				case sf::Keyboard::Down:
					red.move(0, -1);
					break;
				default:
					break;
				}
				break;
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Z:
					up = false;
					break;
				case sf::Keyboard::S:
					down = false;
					break;
				case sf::Keyboard::Q:
					left = false;
					break;
				case sf::Keyboard::D:
					right = false;
					break;
				case sf::Keyboard::Left:
					rotateLeft = false;
					break;
				case sf::Keyboard::Right:
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

		frametime = frameClock.restart().asSeconds();
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
			//red.setPosition(400 + (int)(100 * glm::cos(angle)), 300 + (int)(100 * glm::sin(angle)));
			blue.setPosition(400 + (int)(-100 * glm::cos(angle)), 300 + (int)(-100 * glm::sin(angle)));
			if (red.colides(&blue)) std::cout << "Collision!!!!!" << std::endl;
		}

		window.draw(red);
		window.draw(blue);
		window.draw(green);
		window.draw(text);
		window.display();

		moved = false;
		second += frametime;
		frameCount++;
		if (second > 1.0f) {
			std::cout << frameCount << std::endl;
			second = 0.0f;
			frameCount = 0;
		}
	}

	return 0;
}