#include <MyGL/Drawable/Polygon.hpp>
#include <numeric>

namespace my
{
    std::unordered_map<unsigned int, const Polygon::GLinfo> Polygon::buffers;

    void Polygon::computeVertices(unsigned int sides, GLinfo& buffer) {
        float angle = 0.0f;
        float angle_increment = (2 * pi) / static_cast<float>(sides);

        buffer.vertices = std::vector<float>(5 * (static_cast<size_t>(sides) + 1));

        // the center's coordinates
        buffer.vertices[0] = 0.0f; // first 3 coords are the normalized device coordinates
        buffer.vertices[1] = 0.0f;
        buffer.vertices[2] = 0.0f;
        buffer.vertices[3] = 0.5f; // the last 2 are the texture coordinates
        buffer.vertices[4] = 0.5f;

        for (size_t i = 5; i < buffer.vertices.size(); i += 5) {
            buffer.vertices[i] = glm::cos(angle);
            buffer.vertices[i + 1] = glm::sin(angle);
            buffer.vertices[i + 2] = 0.0f;
            buffer.vertices[i + 3] = (buffer.vertices[i] + 1.0f) / 2.0f;
            buffer.vertices[i + 4] = (buffer.vertices[i + 1] + 1.0f) / 2.0f;
            angle += angle_increment;
        }

        buffer.indices = std::vector<unsigned int>(static_cast<size_t>(sides) + 2);
        std::iota(buffer.indices.begin(), buffer.indices.end(), 0u);
        buffer.indices.back() = 1u;
    }

    void Polygon::glInit(unsigned int sides) {
        if (buffers.find(sides) == buffers.end()) {
            GLinfo buffer;
            computeVertices(sides, buffer);
            glGenVertexArrays(1, &buffer.vao);
            glGenBuffers(1, &buffer.vbo);
            glGenBuffers(1, &buffer.ebo);

            glBindVertexArray(buffer.vao);
            glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);

            glBufferData(GL_ARRAY_BUFFER, buffer.vertices.size() * sizeof(float), buffer.vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.indices.size() * sizeof(unsigned int), buffer.indices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            buffers.insert({sides, buffer});
        }
    }

    std::vector<glm::vec2> Polygon::points() const {
        glm::mat4 transform;
        if (m_updateMatrix) {
            transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(m_position.x, m_position.y, 0.0f));
            transform = glm::rotate(transform, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::scale(transform, glm::vec3(m_originalScale.x * m_scaleFactor.x, m_originalScale.y * m_scaleFactor.y, 1.0f));
        } else {
            transform = m_model;
        }

        std::vector<glm::vec2> p;
        p.reserve(m_sides);
        for (size_t i = 5; i < p_buffer->vertices.size(); i += 5) {
            glm::vec4 vertex = transform * glm::vec4(p_buffer->vertices[i], p_buffer->vertices[i + 1], 0.0f, 1.0f);
            p.emplace_back(glm::vec2(vertex.x, vertex.y));
        }

        return p;
    }

    Polygon::Polygon() : Polygon(3, 10) {}

    Polygon::Polygon(unsigned int sides, int radius) : AbstractShape(2 * radius, 2 * radius), m_sides(sides < 3 ? 3 : sides), p_buffer(nullptr) {
        glInit(m_sides);
        p_buffer = &buffers[m_sides];
    }

    Polygon::Polygon(unsigned int sides, int radius, int x, int y) : Polygon(sides, radius) {
        setPosition(x, y, true);
    }

    Polygon::~Polygon() {}

    void Polygon::setSides(unsigned int sides) {
        m_sides = sides < 3 ? 3 : sides;
        glInit(m_sides);
        p_buffer = &buffers[m_sides];
    }

    void Polygon::draw(const glm::mat4& lookAt, const glm::mat4& projection) const {
        if (m_updateMatrix) {
            m_model = glm::mat4(1.0f);
            m_model = glm::translate(m_model, glm::vec3(m_position.x, m_position.y, 0.0f));
            m_model = glm::rotate(m_model, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            if (m_outlineThickness > 0.0f) {
                m_outlineModel = glm::scale(
                  m_model, glm::vec3(m_outlineThickness + m_originalScale.x * m_scaleFactor.x, m_outlineThickness + m_originalScale.y * m_scaleFactor.y, 1.0f));
            }
            m_model = glm::scale(m_model, glm::vec3(m_originalScale.x * m_scaleFactor.x, m_originalScale.y * m_scaleFactor.y, 1.0f));
            m_updateMatrix = false;
        }

        glActiveTexture(GL_TEXTURE3);
        m_texture.bind();
        m_shader.setMat4("model", m_model);
        m_shader.setMat4("view", lookAt);
        m_shader.setMat4("projection", projection);
        m_shader.setFloat("color", m_color.getNormalized());
        m_shader.use();

        glBindVertexArray(p_buffer->vao);
        if (m_outlineThickness > 0.0f) {
            glClear(GL_STENCIL_BUFFER_BIT);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
            glDrawElements(GL_TRIANGLE_FAN, m_sides + 2, GL_UNSIGNED_INT, nullptr);

            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            m_outlineShader.setMat4("model", m_outlineModel);
            m_outlineShader.setFloat("color", m_outlineColor.getNormalized());
            if (m_shader != m_outlineShader) {
                m_outlineShader.setMat4("view", lookAt);
                m_outlineShader.setMat4("projection", projection);
            }
            m_outlineShader.use();
            glDrawElements(GL_TRIANGLE_FAN, m_sides + 2, GL_UNSIGNED_INT, nullptr);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        } else {
            glDrawElements(GL_TRIANGLE_FAN, m_sides + 2, GL_UNSIGNED_INT, nullptr);
        }
    }
} // namespace my
