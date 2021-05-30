#include <MyGL/mygl.hpp>

int main() {
    std::string vertexSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;"
      "uniform mat4 model;"
      "uniform mat4 view;"
      "uniform mat4 projection;"
      "varying vec3 vColor;"
      "void main(){"
      "    vec4 pos = projection * view * model * vec4(aPos, 1.0);"
      "    gl_Position = pos;"
      "    vColor = vec3(abs(pos.x), abs(pos.y), distance(pos.xy, vec2(0.0,  0.0)));"
      "}";

    my::GLWindow window(800, 600, "Rectangle");
    window.setFramerate(60);

    my::Shader vertexShader;
    vertexShader.loadFromString(vertexSource.c_str(), my::Shader::Type::Vertex);
    my::Shader fragmentShader;
    fragmentShader.loadFromFile("@RESSOURCES_DIR@/CustomFragment.glsl", my::Shader::Type::Fragment);
    my::ShaderProgram program;
    program.addShaders(vertexShader, fragmentShader);
    program.link();

    my::Rectangle rectangle(200, 100);
    rectangle.setPosition(400, 300, true);
    rectangle.setShader(program);

    my::Event event;
    while (window.isRunning()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case my::EventType::windowShouldClose:
                window.close();
                break;

            case my::EventType::keyPressed:
                if (event.keyCode == my::Key::escape) {
                    window.close();
                }
                break;

            case my::EventType::mouseMoved:
                rectangle.setPosition(event.mouse.pos, true);
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