#include <MyGL/Drawable/Polygon.hpp>
using namespace my;

std::unordered_map<unsigned int, const Polygon::GLinfo> Polygon::buffers = {};

void Polygon::computeVertices(unsigned int sides, GLinfo& buffer) {
    float angle = 0.0f;
    float inc = (2 * pi) / sides;

    buffer.vertices = std::vector<float>(static_cast<size_t>(5 * (sides + 1)));
    buffer.indices = std::vector<unsigned int>(static_cast<size_t>(sides + 2));

    //the center's coordinates
    buffer.vertices[0] = 0.0f;
    buffer.vertices[1] = 0.0f;
    buffer.vertices[2] = 0.0f;
    buffer.vertices[3] = 0.5f;
    buffer.vertices[4] = 0.5f;

    for (size_t i = 5; i < buffer.vertices.size(); i += 5) {
        buffer.vertices[i] = glm::cos(angle);
        buffer.vertices[i + 1] = glm::sin(angle);
        buffer.vertices[i + 2] = 0.0f;
        buffer.vertices[i + 3] = (buffer.vertices[i] + 1.0f) / 2.0f;
        buffer.vertices[i + 4] = (buffer.vertices[i + 1] + 1.0f) / 2.0f;
        angle += inc;
    }

    for (size_t i = 0; i < buffer.indices.size(); i++) buffer.indices[i] = static_cast<unsigned int>(i);
    buffer.indices[buffer.indices.size() - 1] = 1;
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.indices.size() * sizeof(unsigned int) , buffer.indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        buffers.insert({ sides, buffer });
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

    std::vector<glm::vec2> res;
    res.reserve(m_sides);
    for (size_t i = 5; i < p_buffer->vertices.size(); i += 5) {
        glm::vec4 vertex = transform * glm::vec4(p_buffer->vertices[i], p_buffer->vertices[i + 1], 0.0f, 1.0f);
        res.emplace_back(glm::vec2(vertex.x, vertex.y));
    }

    return res;
}

Polygon::Polygon() : Polygon(3, 10) {}

Polygon::Polygon(unsigned int sides, int radius) : AbstractShape(radius, radius),
    m_sides(sides < 3 ? 3 : sides), p_buffer(nullptr)
{
    glInit(m_sides);
    p_buffer = &buffers[m_sides];
}

Polygon::Polygon(unsigned int sides, int radius, int x, int y) : Polygon(sides, radius) {
    setPosition(x, y, true);
}

void Polygon::setSides(unsigned int sides) {
    m_sides = sides < 3 ? 3 : sides;
    glInit(m_sides);
    p_buffer = &buffers[m_sides];
}

void Polygon::draw(const glm::mat4& lookAt, const glm::mat4& projection) {
    if (m_updateMatrix) {
        m_model = glm::mat4(1.0f);
        m_model = glm::translate(m_model, glm::vec3(m_position.x, m_position.y, 0.0f));
        m_model = glm::rotate(m_model, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        if (m_outlineThickness > 0.0f) {
            m_outlineModel = glm::scale(m_model, glm::vec3(m_outlineThickness + m_originalScale.x * m_scaleFactor.x, m_outlineThickness + m_originalScale.y * m_scaleFactor.y, 1.0f));
        }
        m_model = glm::scale(m_model, glm::vec3(m_originalScale.x * m_scaleFactor.x, m_originalScale.y * m_scaleFactor.y, 1.0f));
        m_updateMatrix = false;
    }
    glActiveTexture(GL_TEXTURE3);
    m_texture.bind();
    p_activeShader->setMat4("model", m_model);
    p_activeShader->setMat4("view", lookAt);
    p_activeShader->setMat4("projection", projection);
    p_activeShader->setFloat("color", m_color.getNormalized());
    p_activeShader->use();

    if (m_outlineThickness > 0.0f) {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glBindVertexArray(p_buffer->vao);
        glDrawElements(GL_TRIANGLE_FAN, m_sides + 2, GL_UNSIGNED_INT, 0);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        shader.setMat4("model", m_outlineModel);
        shader.setFloat("color", m_outlineColor.getNormalized());
        if (m_isTextured) {
            shader.setMat4("view", lookAt);
            shader.setMat4("projection", projection);
        }
        shader.use();

        glDrawElements(GL_TRIANGLE_FAN, m_sides + 2, GL_UNSIGNED_INT, 0);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    } else {
        glBindVertexArray(p_buffer->vao);
        glDrawElements(GL_TRIANGLE_FAN, m_sides + 2, GL_UNSIGNED_INT, 0);
    }
    glClear(GL_STENCIL_BUFFER_BIT);
}