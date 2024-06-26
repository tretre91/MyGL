#include <MyGL/Drawable/AbstractShape.hpp>
#include <algorithm>

constexpr const char* vertexSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;"
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 projection;"
  "void main(){"
  "    gl_Position = projection * view * model * vec4(aPos.x, -aPos.y, aPos.z, 1.0);"
  "}";

constexpr const char* fragmentSource =
  "#version 330 core\n"
  "out vec4 FragColor;"
  "uniform vec4 color;"
  "void main(){"
  "    FragColor = color;"
  "}";

constexpr const char* texVertexSource =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;"
  "layout (location = 1) in vec2 aTexCoords;"
  "out vec2 texCoords;"
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 projection;"
  "void main(){"
  "    gl_Position = projection * view * model * vec4(aPos.x, -aPos.y, aPos.z, 1.0);"
  "    texCoords = aTexCoords;"
  "}";

constexpr const char* texFragmentSource =
  "#version 330 core\n"
  "in vec2 texCoords;"
  "out vec4 FragColor;"
  "uniform sampler2D tex;"
  "uniform vec4 color;"
  "void main(){"
  "    FragColor = texture(tex, texCoords);"
  "}";

namespace my
{
    ShaderProgram AbstractShape::shader;
    ShaderProgram AbstractShape::texShader;
    const ShaderProgram& AbstractShape::defaultShader = AbstractShape::shader;

    void AbstractShape::initShaders() {
        if (!shader.isUsable()) {
            shader.addShaders(Shader(vertexSource, Shader::Type::Vertex), Shader(fragmentSource, Shader::Type::Fragment));
            shader.link();
        }
        if (!texShader.isUsable()) {
            texShader.addShaders(Shader(texVertexSource, Shader::Type::Vertex), Shader(texFragmentSource, Shader::Type::Fragment));
            texShader.link();
            texShader.setInt("tex", 3);
        }
    }

    AbstractShape::AbstractShape() :
      m_position(0.0f, 0.0f), m_originalScale(5.0f, 5.0f), m_scaleFactor(1.0f, 1.0f), m_rotationAngle(0.0f), m_updateMatrix(true), m_model(1.0f),
      m_color(100, 100, 100), m_outlineThickness(0.0f), m_outlineColor(255, 255, 255), m_outlineModel(1.0f), m_isTextured(false) {
        initShaders();
        m_shader = shader;
        m_outlineShader = shader;
    }

    AbstractShape::AbstractShape(int width, int height) :
      m_position(0.0f, 0.0f), m_originalScale(static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f), m_scaleFactor(1.0f, 1.0f),
      m_rotationAngle(0.0f), m_updateMatrix(true), m_model(1.0f), m_color(100, 100, 100), m_outlineThickness(0.0f), m_outlineColor(255, 255, 255),
      m_outlineModel(1.0f), m_isTextured(false) {
        initShaders();
        m_shader = shader;
        m_outlineShader = shader;
    }

    AbstractShape::AbstractShape(int width, int height, int x, int y) : AbstractShape(width, height) {
        setPosition(x, y, true);
    }

    AbstractShape::~AbstractShape() {}

    glm::vec2 AbstractShape::getSize() const noexcept {
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

    void AbstractShape::move(float x, float y) noexcept {
        m_position.x += x;
        m_position.y += y;
        m_updateMatrix = true;
    }

    glm::vec2 AbstractShape::getPosition() const noexcept {
        return m_position;
    }

    void AbstractShape::setScale(float x, float y) noexcept {
        m_scaleFactor.x = x;
        m_scaleFactor.y = y;
        m_updateMatrix = true;
    }

    void AbstractShape::scale(float x, float y) noexcept {
        m_scaleFactor.x *= x;
        m_scaleFactor.y *= y;
        m_updateMatrix = true;
    }

    glm::vec2 AbstractShape::getScale() const noexcept {
        return m_scaleFactor;
    }

    void AbstractShape::setRotation(float angle) {
        m_rotationAngle = glm::mod(angle, 360.0f); // TODO : check if glm::mod can throw
        m_updateMatrix = true;
    }

    void AbstractShape::rotate(float angle) {
        m_rotationAngle = glm::mod(m_rotationAngle + angle, 360.0f); // TODO : check if glm::mod can throw
        m_updateMatrix = true;
    }

    float AbstractShape::getRotation() const noexcept {
        return m_rotationAngle;
    }

    void AbstractShape::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) noexcept {
        m_color = my::Color(r, g, b, alpha);
    }

    void AbstractShape::setColor(const my::Color& color) noexcept {
        m_color = color;
    }

    my::Color AbstractShape::getColor() const noexcept {
        return m_color;
    }

    void AbstractShape::setOutlineThickness(unsigned int thickness) {
        m_outlineThickness = static_cast<float>(thickness);
        if (!m_updateMatrix && m_outlineThickness > 0.0f) {
            m_outlineModel = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0.0f));
            m_outlineModel = glm::rotate(m_outlineModel, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            m_outlineModel = glm::scale(m_outlineModel,
              glm::vec3(m_outlineThickness + m_originalScale.x * m_scaleFactor.x, m_outlineThickness + m_originalScale.y * m_scaleFactor.y, 1.0f));
        }
    }

    void AbstractShape::setOutlineColor(const my::Color& color) noexcept {
        m_outlineColor = color;
    }

    void AbstractShape::setOutlineColor(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) noexcept {
        m_outlineColor = my::Color(r, g, b, alpha);
    }

    bool AbstractShape::SATCollides(const AbstractShape& shape) const {
        // We test if the shapes are close enough to potentialy touch
        const float dist = glm::distance(this->m_position, shape.m_position);
        glm::vec2 sides = m_originalScale * m_scaleFactor;
        const float r1 = glm::sqrt(sides.x * sides.x + sides.y * sides.y);
        sides = shape.m_originalScale * shape.m_scaleFactor;
        const float r2 = glm::sqrt(sides.x * sides.x + sides.y * sides.y);
        if (dist > r1 + r2) {
            return false;
        }

        // if the shapes are close enough we use the separating axis
        // theorem to test wether they are coliding
        std::vector<glm::vec2> points1 = this->points();
        points1.push_back(points1[0]);
        std::vector<glm::vec2> points2 = shape.points();
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

    bool AbstractShape::BBoxCollides(const AbstractShape& shape) const noexcept {
        glm::vec2 thisScale = this->m_originalScale * this->m_scaleFactor;
        glm::vec2 shapeScale = shape.m_originalScale * shape.m_scaleFactor;

        return std::abs(this->m_position.y - shape.m_position.y) < std::abs(thisScale.y) + abs(shapeScale.y)
               && std::abs(this->m_position.x - shape.m_position.x) < std::abs(thisScale.x) + abs(shapeScale.x);
    }

    void AbstractShape::setTexture(const my::Texture& texture) {
        m_texture = texture;
        m_shader = texShader;
        m_isTextured = true;
    }

    void AbstractShape::setTexture(const std::string& filename) {
        m_texture = my::Texture(filename);
        m_texture.setTextureWrapMethod(Texture::Axis::s, GL_REPEAT);
        m_texture.setTextureWrapMethod(Texture::Axis::t, GL_REPEAT);
        m_shader = texShader;
        m_isTextured = true;
    }

    void AbstractShape::setShader(const ShaderProgram& program) {
        if (program.isUsable()) {
            m_shader = program;
        }
    }

    void AbstractShape::setOutlineShader(const ShaderProgram& program) {
        if (program.isUsable()) {
            m_outlineShader = program;
        }
    }
} // namespace my
