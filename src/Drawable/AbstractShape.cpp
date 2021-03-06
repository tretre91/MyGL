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

AbstractShape::AbstractShape() : m_position(0.0f, 0.0f), m_originalScale(5.0f, 5.0f), m_scaleFactor(1.0f, 1.0f),
    m_rotationAngle(0.0f), m_updateMatrix(true), m_model(1.0f), m_color(100, 100, 100), m_outlineThickness(0),
    m_outlineColor(255, 255, 255), m_outlineModel(1.0f), m_texture(), m_isTextured(false), p_activeShader(nullptr)
{
    if (!shaderIsUsable) {
        shader = my::Shader(vertexSource, fragmentSource, false);
        texShader = my::Shader(texVertexSource, texFragmentSource, false);
        texShader.setInt("tex", 3);
        shaderIsUsable = true;
    }
    p_activeShader = &shader;
}

AbstractShape::AbstractShape(int width, int height) : m_position(0.0f, 0.0f), m_originalScale(width / 2.0f, height / 2.0f),
    m_scaleFactor(1.0f, 1.0f), m_rotationAngle(0.0f), m_updateMatrix(true), m_model(1.0f), m_color(100, 100, 100), m_outlineThickness(0),
    m_outlineColor(255, 255, 255), m_outlineModel(1.0f), m_texture(), m_isTextured(false), p_activeShader(nullptr)
{
    if (!shaderIsUsable) {
        shader = my::Shader(vertexSource, fragmentSource, false);
        texShader = my::Shader(texVertexSource, texFragmentSource, false);
        texShader.setInt("tex", 3);
        shaderIsUsable = true;
    }
    p_activeShader = &shader;
}

AbstractShape::AbstractShape(int width, int height, int x, int y) : AbstractShape(width, height) {
    setPosition(x, y, true);
}


void AbstractShape::setPosition(int x, int y, bool center) {
    m_position.x = static_cast<float>(x);
    m_position.y = static_cast<float>(y);
    if (!center) {
        m_position.x += m_originalScale.x * m_scaleFactor.x;
        m_position.y -= m_originalScale.y * m_scaleFactor.y;
    }
    m_updateMatrix = true;
}

void AbstractShape::setPosition(const glm::vec2& pos, bool center) {
    m_position = pos;
    if (!center) {
        m_position.x += m_originalScale.x * m_scaleFactor.x;
        m_position.y -= m_originalScale.y * m_scaleFactor.y;
    }
    m_updateMatrix = true;
}

void AbstractShape::move(float x, float y) {
    m_position.x += x;
    m_position.y += y;
    m_updateMatrix = true;
}

glm::vec2 AbstractShape::getPosition() const {
    return m_position;
}


void AbstractShape::setScale(float x, float y) {
    m_scaleFactor.x = x;
    m_scaleFactor.y = y;
    m_updateMatrix = true;
}

void AbstractShape::scale(float x, float y) {
    m_scaleFactor.x *= x;
    m_scaleFactor.y *= y;
    m_updateMatrix = true;
}

glm::vec2 AbstractShape::getScale() const {
    return m_scaleFactor;
}


void AbstractShape::setRotation(float angle) {
    m_rotationAngle = glm::mod(angle, 360.0f);
    m_updateMatrix = true;
}

void AbstractShape::rotate(float angle) {
    m_rotationAngle = glm::mod(m_rotationAngle + angle, 360.0f);
    m_updateMatrix = true;
}

float AbstractShape::getRotation() const {
    return m_rotationAngle;
}


void AbstractShape::setColor(int r, int g, int b, int alpha) {
    r = r < 0 ? 0 : r % 255;
    g = g < 0 ? 0 : g % 255;
    b = b < 0 ? 0 : b % 255;
    alpha = alpha < 0 ? 0 : alpha % 255;
    m_color = my::Color(r, g, b, alpha);
}

void AbstractShape::setColor(const my::Color& color) {
    this->m_color = color;
}

my::Color AbstractShape::getColor() const {
    return m_color;
}

void AbstractShape::setOutlineThickness(unsigned int thickness) {
    m_outlineThickness = thickness;
    if (!m_updateMatrix && m_outlineThickness > 0) {
        m_outlineModel = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0.0f));
        m_outlineModel = glm::rotate(m_outlineModel, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        m_outlineModel = glm::scale(m_outlineModel, glm::vec3(static_cast<float>(m_outlineThickness) + m_originalScale.x * m_scaleFactor.x, static_cast<float>(m_outlineThickness) + m_originalScale.y * m_scaleFactor.y, 1.0f));
    }
}

void AbstractShape::setOutlineColor(const my::Color& color) {
    m_outlineColor = color;
}

void AbstractShape::setOutlineColor(int r, int g, int b, int alpha) {
    r = r > 255 ? 255 : (r >= 0) * r;
    g = g > 255 ? 255 : (g >= 0) * g;
    b = b > 255 ? 255 : (b >= 0) * b;
    alpha = alpha > 255 ? 255 : (alpha >= 0) * alpha;
    m_outlineColor = my::Color(r, g, b, alpha);
}

bool AbstractShape::SATCollides(const AbstractShape& otherShape) const {
    const AbstractShape* shape = &otherShape;
    // We test if the shapes are close enough to potentialy touch
    float dist = glm::distance(this->m_position, shape->m_position);
    glm::vec2 radius = m_originalScale * m_scaleFactor;
    float r1 = std::max(radius.x, radius.y);
    radius = shape->m_originalScale * shape->m_scaleFactor;
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
    glm::vec2 thisScale = this->m_originalScale * this->m_scaleFactor;
    glm::vec2 shapeScale = shape->m_originalScale * shape->m_scaleFactor;

    return abs(this->m_position.y - shape->m_position.y) < abs(thisScale.y) + abs(shapeScale.y)
           && abs(this->m_position.x - shape->m_position.x) < abs(thisScale.x) + abs(shapeScale.x);
}

void AbstractShape::setTexture(const my::Texture& texture) {
    this->m_texture = texture;
    p_activeShader = &texShader;
    m_isTextured = true;
}

void AbstractShape::setTexture(const std::string& filename, bool hasAlpha) {
    m_texture = my::Texture(filename, hasAlpha ? GL_RGBA : GL_RGB);
    m_texture.setTextureWrapMethod(my::Texture::Axis::s, GL_REPEAT);
    m_texture.setTextureWrapMethod(my::Texture::Axis::t, GL_REPEAT);
    p_activeShader = &texShader;
    m_isTextured = true;
}