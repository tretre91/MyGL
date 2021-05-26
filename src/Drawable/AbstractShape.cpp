#include <MyGL/Drawable/AbstractShape.hpp>
#include <algorithm>
using namespace my;

const float AbstractShape::pi = 3.1415926535f;

const std::string AbstractShape::vertexSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;"
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 projection;"
  "void main(){"
  "    gl_Position = projection * view * model * vec4(aPos, 1.0);"
  "}";

const std::string AbstractShape::fragmentSource =
  "#version 330 core\n"
  "out vec4 FragColor;"
  "uniform vec4 color;"
  "void main(){"
  "    FragColor = color;"
  "}";

const std::string AbstractShape::texVertexSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;"
  "layout (location = 1) in vec2 aTexCoords;"
  "out vec2 texCoords;"
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 projection;"
  "void main(){"
  "    gl_Position = projection * view * model * vec4(aPos, 1.0);"
  "    texCoords = aTexCoords;"
  "}";

const std::string AbstractShape::texFragmentSource =
  "#version 330 core\n"
  "in vec2 texCoords;"
  "out vec4 FragColor;"
  "uniform sampler2D tex;"
  "uniform vec4 color;"
  "void main(){"
  "    FragColor = texture(tex, texCoords);"
  "}";

my::Shader AbstractShape::shader = my::Shader();
my::Shader AbstractShape::texShader = my::Shader();

bool AbstractShape::shaderIsUsable = false;

AbstractShape::AbstractShape() :
  m_position(0.0f, 0.0f), m_originalScale(5.0f, 5.0f), m_scaleFactor(1.0f, 1.0f), m_rotationAngle(0.0f), m_updateMatrix(true), m_model(1.0f),
  m_color(100, 100, 100), m_outlineThickness(0.0f), m_outlineColor(255, 255, 255), m_outlineModel(1.0f), m_isTextured(false), p_activeShader(nullptr) {
    if (!shaderIsUsable) {
        shader = my::Shader(vertexSource, fragmentSource, false);
        texShader = my::Shader(texVertexSource, texFragmentSource, false);
        texShader.setInt("tex", 3);
        shaderIsUsable = true;
    }
    p_activeShader = &shader;
}

AbstractShape::AbstractShape(int width, int height) :
  m_position(0.0f, 0.0f), m_originalScale(width / 2.0f, height / 2.0f), m_scaleFactor(1.0f, 1.0f), m_rotationAngle(0.0f), m_updateMatrix(true), m_model(1.0f),
  m_color(100, 100, 100), m_outlineThickness(0.0f), m_outlineColor(255, 255, 255), m_outlineModel(1.0f), m_isTextured(false), p_activeShader(nullptr) {
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

glm::vec2 AbstractShape::getSize() const {
    return 2.0f * m_originalScale * m_scaleFactor;
}

void AbstractShape::setPosition(int x, int y, bool center) {
    m_position.x = static_cast<float>(x);
    m_position.y = static_cast<float>(y);
    if (!center) {
        m_position.x += m_originalScale.x * m_scaleFactor.x;
        m_position.y += m_originalScale.y * m_scaleFactor.y;
    }
    m_updateMatrix = true;
}

void AbstractShape::setPosition(const glm::vec2& pos, bool center) {
    m_position = pos;
    if (!center) {
        m_position.x += m_originalScale.x * m_scaleFactor.x;
        m_position.y += m_originalScale.y * m_scaleFactor.y;
    }
    m_updateMatrix = true;
}

void AbstractShape::setRelativePosition(int x, int y, const my::Camera& camera, bool center) {
    setRelativePosition(glm::vec2(x, y), camera, center);
}

void AbstractShape::setRelativePosition(const glm::vec2& pos, const my::Camera& camera, bool center) {
    setPosition(camera.getPosition() + pos, center);
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
    m_color = my::Color(glm::clamp(r, 0, 255), glm::clamp(g, 0, 255), glm::clamp(b, 0, 255), glm::clamp(alpha, 0, 255));
}

void AbstractShape::setColor(const my::Color& color) {
    m_color = color;
}

my::Color AbstractShape::getColor() const {
    return m_color;
}

void AbstractShape::setOutlineThickness(unsigned int thickness) {
    m_outlineThickness = static_cast<float>(thickness);
    if (!m_updateMatrix && m_outlineThickness > 0.0f) {
        m_outlineModel = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0.0f));
        m_outlineModel = glm::rotate(m_outlineModel, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        m_outlineModel = glm::scale(
          m_outlineModel, glm::vec3(m_outlineThickness + m_originalScale.x * m_scaleFactor.x, m_outlineThickness + m_originalScale.y * m_scaleFactor.y, 1.0f));
    }
}

void AbstractShape::setOutlineColor(const my::Color& color) {
    m_outlineColor = color;
}

void AbstractShape::setOutlineColor(int r, int g, int b, int alpha) {
    m_outlineColor = my::Color(glm::clamp(r, 0, 255), glm::clamp(g, 0, 255), glm::clamp(b, 0, 255), glm::clamp(alpha, 0, 255));
}

bool AbstractShape::SATCollides(const AbstractShape& otherShape) const {
    const AbstractShape* shape = &otherShape;
    // We test if the shapes are close enough to potentialy touch
    float dist = glm::distance(this->m_position, shape->m_position);
    glm::vec2 radius = m_originalScale * m_scaleFactor;
    float r1 = std::max(radius.x, radius.y);
    radius = shape->m_originalScale * shape->m_scaleFactor;
    float r2 = std::max(radius.x, radius.y);
    if (dist > r1 + r2) {
        return false;
    }

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

            if (*(shape2Range.first) > *(shape1Range.second) || *(shape2Range.second) < *(shape1Range.first)) {
                return false;
            }
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

void AbstractShape::setTexture(const std::string& filename) {
    m_texture = my::Texture(filename);
    m_texture.setTextureWrapMethod(my::Texture::Axis::s, GL_REPEAT);
    m_texture.setTextureWrapMethod(my::Texture::Axis::t, GL_REPEAT);
    p_activeShader = &texShader;
    m_isTextured = true;
}