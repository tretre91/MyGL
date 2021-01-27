#include <MyGL/Drawable/AbstractShape.hpp>
using namespace my;

const float AbstractShape::pi = 3.14159265359;

const std::string AbstractShape::vertexSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main(){\n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"};";

const std::string AbstractShape::fragmentSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main(){\n"
"    FragColor = color;\n"
"};";

my::Shader AbstractShape::shader = my::Shader();

bool AbstractShape::shaderIsUsable = false;

AbstractShape::AbstractShape() : position(0, 0), originalScale(10, 10), scaleFactor(1.0f, 1.0f), rotationAngle(0), updateMatrix(true), color(100, 100, 100), model(1.0f) {
    if (!shaderIsUsable) {
        shader = my::Shader(vertexSource, fragmentSource, false);
        shaderIsUsable = true;
    }
}

AbstractShape::AbstractShape(int width, int height) : position(0, 0), originalScale(width == 1 ? 1 : width / 2, height == 1 ? 1 : height / 2), scaleFactor(1.0f, 1.0f), rotationAngle(0), updateMatrix(true), color(100, 100, 100), model(1.0f) {
    if (!shaderIsUsable) {
        shader = my::Shader(vertexSource, fragmentSource, false);
        shaderIsUsable = true;
    }
}

AbstractShape::AbstractShape(int width, int height, int x, int y) : AbstractShape(width, height) {
    position.x = x;
    position.y = y;
}


void AbstractShape::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
    updateMatrix = true;
}

void AbstractShape::move(int x, int y) {
    position.x += x;
    position.y += y;
    updateMatrix = true;
}

my::Vec2 AbstractShape::getPosition() const {
    return position;
}


void AbstractShape::setScale(float x, float y) {
    scaleFactor.x = x;
    scaleFactor.y = y;
    updateMatrix = true;
}

void AbstractShape::scale(float x, float y) {
    scaleFactor.x += x;
    scaleFactor.y += y;
    updateMatrix = true;
}

glm::vec2 AbstractShape::getScale() const {
    return scaleFactor;
}


void AbstractShape::setRotation(int angle) {
    rotationAngle = angle % 360;
    updateMatrix = true;
}

void AbstractShape::rotate(int angle) {
    rotationAngle += angle;
    rotationAngle %= 360;
    updateMatrix = true;
}

int AbstractShape::getRotation() const {
    return rotationAngle;
}


void AbstractShape::setColor(int r, int g, int b, int alpha) {
    r = r > 255 ? 255 : (r >= 0) * r;
    g = g > 255 ? 255 : (g >= 0) * g;
    b = b > 255 ? 255 : (b >= 0) * b;
    alpha = alpha > 255 ? 255 : (alpha >= 0) * alpha;
    color = my::Color(r, g, b, alpha);
}

void AbstractShape::setColor(const my::Color& color) {
    this->color = color;
}

my::Color AbstractShape::getColor() const {
    return color;
}


bool AbstractShape::colides(AbstractShape* shape) const {
    std::vector<glm::vec2> points1 = this->points();
    points1.push_back(points1[0]);
    std::vector<glm::vec2> points2 = shape->points();
    points2.push_back(points2[0]);


    auto dot = [](glm::vec2 v1, glm::vec2 v2) -> float {
        return v1.x * v2.x + v1.y * v2.y;
    };

    float shape1Range[2];
    float shape2Range[2];

    for (size_t i = 0; i < points1.size() - 1; i++) {
        glm::vec2 axis = points1[i] - points1[i + 1];
        glm::vec2 middle((points1[i].x + points1[i + 1].x) / 2, (points1[i].y + points1[i + 1].y) / 2);
        axis = glm::vec2(axis.y, -axis.x);
        
        std::vector<float> projections;
        for (auto v : points1) projections.push_back(dot(v - middle, axis));
        shape1Range[0] = *std::min_element(projections.begin(), projections.end());
        shape1Range[1] = *std::max_element(projections.begin(), projections.end());

        projections.clear();
        for (auto v : points2) projections.push_back(dot(v - middle, axis));
        shape2Range[0] = *std::min_element(projections.begin(), projections.end());
        shape2Range[1] = *std::max_element(projections.begin(), projections.end());

        if (shape2Range[0] > shape1Range[1] || shape2Range[1] < shape1Range[0]) return false;
    }

    for (size_t i = 0; i < points2.size() - 1; i++) {
        glm::vec2 axis = points2[i] - points2[i + 1];
        glm::vec2 middle((points2[i].x + points2[i + 1].x) / 2, (points2[i].y + points2[i + 1].y) / 2);
        axis = glm::vec2(axis.y, -axis.x);

        std::vector<float> projections;
        for (auto v : points1) projections.push_back(dot(v - middle, axis));
        shape1Range[0] = *std::min_element(projections.begin(), projections.end());
        shape1Range[1] = *std::max_element(projections.begin(), projections.end());

        projections.clear();
        for (auto v : points2) projections.push_back(dot(v - middle, axis));
        shape2Range[0] = *std::min_element(projections.begin(), projections.end());
        shape2Range[1] = *std::max_element(projections.begin(), projections.end());

        if (shape2Range[0] > shape1Range[1] || shape2Range[1] < shape1Range[0]) return false;
    }

    return true;
}