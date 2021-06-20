#include <MyGL/mygl.hpp>

int main() {
    const std::string vertexSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;"
      "uniform mat4 model;"
      "uniform mat4 view;"
      "uniform mat4 projection;"
      "varying vec3 vColor;"
      "void main(){"
      "    vec4 pos = projection * view * model * vec4(aPos, 1.0);"
      "    gl_Position = pos;"
      "    vColor = vec3(abs(pos.x), abs(pos.y), distance(pos.xy, vec2(0.0, 0.0)));"
      "}";

    my::GLWindow window(800, 600, "Custom shader");
    window.setFramerate(60);

    my::Shader vertexShader;
    vertexShader.loadFromString(vertexSource.c_str(), my::Shader::Type::Vertex);
    my::Shader fragmentShader;
    fragmentShader.loadFromFile("@RESOURCES_DIR@/CustomFragment.glsl", my::Shader::Type::Fragment);
    my::ShaderProgram program;
    program.addShaders(vertexShader, fragmentShader);
    program.link();

    my::Triangle triangle(250, 150, 550, 150, 400, 400);
    triangle.setPosition(400, 300, true);
    triangle.setShader(program);

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
                triangle.setPosition(event.mouse.pos, true);
                break;

            default:
                break;
            }
        }

        window.clear(my::Color::black);

        window.draw(triangle);

        window.display();
    }
}