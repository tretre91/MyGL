#include <MyGL/Drawable/AbstractShape.hpp>
using namespace my;

const float AbstractShape::pi = 3.1415926535f;

const std::string AbstractShape::vertexSource ="\
#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
uniform mat4 model;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
void main(){\n\
    gl_Position = projection * view * model * vec4(aPos, 1.0);\n\
};";

const std::string AbstractShape::fragmentSource ="\
#version 330 core\n\
out vec4 FragColor;\n\
uniform vec4 color;\n\
void main(){\n\
    FragColor = color;\n\
};";

const std::string AbstractShape::texVertexSource ="\
#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
layout (location = 1) in vec2 aTexCoords;\n\
out vec2 texCoords;\n\
uniform mat4 model;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
void main(){\n\
    gl_Position = projection * view * model * vec4(aPos, 1.0);\n\
    texCoords = aTexCoords;\n\
};";

const std::string AbstractShape::texFragmentSource ="\
#version 330 core\n\
in vec2 texCoords;\n\
out vec4 FragColor;\n\
uniform sampler2D tex;\n\
uniform vec4 color;\n\
void main(){\n\
    FragColor = texture(tex, texCoords);\n\
};";

my::Shader AbstractShape::shader = my::Shader();
my::Shader AbstractShape::texShader = my::Shader();

bool AbstractShape::shaderIsUsable = false;

AbstractShape::AbstractShape() : position(0.0f, 0.0f), originalScale(5.0f, 5.0f), scaleFactor(1.0f, 1.0f),
rotationAngle(0), updateMatrix(true), color(100, 100, 100), model(1.0f), texture()
{
    if (!shaderIsUsable) {
        shader = my::Shader(vertexSource, fragmentSource, false);
        texShader = my::Shader(texVertexSource, texFragmentSource, false);
        texShader.setInt("tex", 3);
        shaderIsUsable = true;
    }
    activeShader = &shader;
}

AbstractShape::AbstractShape(int width, int height) : position(0.0f, 0.0f), originalScale(width / 2.0f, height / 2.0f),
scaleFactor(1.0f, 1.0f), rotationAngle(0), updateMatrix(true), color(100, 100, 100), model(1.0f), texture()
{
    if (!shaderIsUsable) {
        shader = my::Shader(vertexSource, fragmentSource, false);
        texShader = my::Shader(texVertexSource, texFragmentSource, false);
        texShader.setInt("tex", 3);
        shaderIsUsable = true;
    }
    activeShader = &shader;
}

AbstractShape::AbstractShape(int width, int height, int x, int y) : AbstractShape(width, height) {
    setPosition(x, y, true);
}


void AbstractShape::setPosition(int x, int y, bool center) {
    position.x = static_cast<float>(x);
    position.y = static_cast<float>(y);
    if (!center) {
        position.x += originalScale.x * scaleFactor.x;
        position.y -= originalScale.y * scaleFactor.y;
    }
    updateMatrix = true;
}

void AbstractShape::setPosition(const glm::vec2& pos, bool center) {
    position = pos;
    if (!center) {
        position.x += originalScale.x * scaleFactor.x;
        position.y -= originalScale.y * scaleFactor.y;
    }
    updateMatrix = true;
}

void AbstractShape::move(float x, float y) {
    position.x += x;
    position.y += y;
    updateMatrix = true;
}

glm::vec2 AbstractShape::getPosition() const {
    return position;
}


void AbstractShape::setScale(float x, float y) {
    scaleFactor.x = x;
    scaleFactor.y = y;
    updateMatrix = true;
}

void AbstractShape::scale(float x, float y) {
    scaleFactor.x *= x;
    scaleFactor.y *= y;
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


bool AbstractShape::SATCollides(const AbstractShape& otherShape) const {
    const AbstractShape* shape = &otherShape;
    // We test if the shapes are close enough to potentialy touch
    float dist = glm::distance(this->position, shape->position);
    glm::vec2 radius = originalScale * scaleFactor;
    float r1 = std::max(radius.x, radius.y);
    radius = shape->originalScale * shape->scaleFactor;
    float r2 = std::max(radius.x, radius.y);
    if(dist > r1 + r2) return false;

    // if the shapes are close enough we use the separating axis
    // theorem to test wether they are coliding
    std::vector<glm::vec2> points1 = this->points();
    points1.push_back(points1[0]);
    std::vector<glm::vec2> points2 = shape->points();
    points2.push_back(points2[0]);

    glm::vec2 axis;
    glm::vec2 middle;
    auto dot = [&middle, &axis](const glm::vec2& v) -> float { return glm::dot(v - middle, axis); };

    std::vector<float> projections1(points1.size());
    std::vector<float> projections2(points2.size());
    std::vector<glm::vec2>* currentShape = &points1;

    // the outer loop allows to not write a second time the code of the inner loop
    for (int s = 0; s < 2; s++) {
        for (size_t i = 0; i < currentShape->size() - 1; i++) {
            axis = (*currentShape)[i] - (*currentShape)[i + 1];
            middle = ((*currentShape)[i] + (*currentShape)[i + 1]) / 2.0f;
            axis = glm::normalize(glm::vec2(axis.y, -axis.x));
            
            std::transform(points1.begin(), points1.end(), projections1.begin(), dot);
            auto shape1Range = std::minmax_element(projections1.begin(), projections1.end());

            std::transform(points2.begin(), points2.end(), projections2.begin(), dot);
            auto shape2Range = std::minmax_element(projections2.begin(), projections2.end());

            if (*(shape2Range.first) > *(shape1Range.second) || *(shape2Range.second) < *(shape1Range.first)) return false;
        }
        currentShape = &points2;
    }

    return true;
}

bool AbstractShape::BBoxCollides(const AbstractShape& otherShape) const {
    const AbstractShape* shape = &otherShape;
    glm::vec2 thisScale = this->originalScale * this->scaleFactor;
    glm::vec2 shapeScale = shape->originalScale * shape->scaleFactor;

    return abs(this->position.y - shape->position.y) < abs(thisScale.y) + abs(shapeScale.y)
           && abs(this->position.x - shape->position.x) < abs(thisScale.x) + abs(shapeScale.x);
}

void AbstractShape::setTexture(const std::string& filename, bool hasAlpha) {
    texture = my::Texture(filename, hasAlpha ? GL_RGBA : GL_RGB);
    texture.setTextureWrapMethod(my::Texture::Axis::s, GL_REPEAT);
    texture.setTextureWrapMethod(my::Texture::Axis::t, GL_REPEAT);
    activeShader = &texShader;
}