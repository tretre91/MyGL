#include <MyGL/Drawable/Triangle.hpp>
#include <algorithm>
#include <glad/glad.h>
using namespace my;

std::vector<glm::vec2> my::Triangle::points() const {
    glm::mat4 transform;
    if (m_updateMatrix) {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(m_position.x, m_position.y, 0.0f));
        transform = glm::rotate(transform, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(m_originalScale.x * m_scaleFactor.x, m_originalScale.y * m_scaleFactor.y, 1.0f));
    } else {
        transform = m_model;
    }

    std::vector<glm::vec2> points(3);
    for (size_t i = 0; i < points.size(); i++) {
        glm::vec4 vertex = transform * glm::vec4(m_vertices[5 * i], m_vertices[5 * i + 1], 0.0f, 1.0f);
        points[i] = glm::vec2(vertex.x, vertex.y);
    }

    return points;
}

Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3) : m_vao(0), m_vbo(0), m_vertices{} {
    std::array<float, 3> x_coords{x1, x2, x3};
    std::array<float, 3> y_coords{y1, y2, y3};
    const auto xBounds = std::minmax_element(x_coords.begin(), x_coords.end());
    const float xMin = *xBounds.first;
    const float xMax = *xBounds.second;
    const auto yBounds = std::minmax_element(y_coords.begin(), y_coords.end());
    const float yMin = *yBounds.first;
    const float yMax = *yBounds.second;

    const float x_dist = glm::distance(xMin, xMax);
    const float y_dist = glm::distance(yMin, yMax);

    for (size_t i = 0; i < 3; i++) {
        x_coords[i] = 2 * (x_coords[i] - xMin) / x_dist - 1;
        y_coords[i] = 2 * (y_coords[i] - yMin) / y_dist - 1;
        m_vertices[i * 5] = x_coords[i];
        m_vertices[i * 5 + 1] = y_coords[i];
        m_vertices[i * 5 + 2] = 0.0f;
        m_vertices[i * 5 + 3] = (x_coords[i] + 1) / 2.0f;
        m_vertices[i * 5 + 4] = (y_coords[i] + 1) / 2.0f;
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    m_originalScale = {x_dist / 2.0f, y_dist / 2.0f};
    Triangle::setPosition(glm::vec2(xMin, yMin));
}

Triangle::Triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) : Triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y) {}

void Triangle::setPosition(int x, int y, bool center) {
    setPosition(glm::vec2(static_cast<float>(x), static_cast<float>(y)), center);
}

void Triangle::setPosition(const glm::vec2& pos, bool center) {
    m_position = pos;
    m_position.x += m_originalScale.x * m_scaleFactor.x;
    m_position.y += m_originalScale.y * m_scaleFactor.y;
    if (center) {
        const auto pts(points());
        const float xMin = std::min({pts[0].x, pts[1].x, pts[2].x});
        const float yMin = std::min({pts[0].y, pts[1].y, pts[2].y});
        const glm::vec2 centroid((pts[0] + pts[1] + pts[2]) / 3.0f);
        m_position.x -= centroid.x - xMin;
        m_position.y -= centroid.y - yMin;
    }
    m_updateMatrix = true;
}

void Triangle::draw(const glm::mat4& lookAt, const glm::mat4& projection) {
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

    glBindVertexArray(m_vao);
    if (m_outlineThickness > 0.0f) {
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        m_outlineShader.setMat4("model", m_outlineModel);
        m_outlineShader.setFloat("color", m_outlineColor.getNormalized());
        if (m_shader != m_outlineShader) {
            m_outlineShader.setMat4("view", lookAt);
            m_outlineShader.setMat4("projection", projection);
        }
        m_outlineShader.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}