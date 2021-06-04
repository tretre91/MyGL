#include <MyGL/Drawable/Rectangle.hpp>
using namespace my;

unsigned int Rectangle::VAO = 0, Rectangle::VBO = 0, Rectangle::EBO = 0;

const std::array<float, 20> Rectangle::vertices = {
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
  -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
  1.0f, 1.0f, 0.0f, 1.0f, 1.0f    // top right
};

const std::array<unsigned int, 4> Rectangle::indices = {0, 1, 2, 3};

void Rectangle::glInit() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

Rectangle::Rectangle() {
    glInit();
}

Rectangle::Rectangle(int width, int height) : AbstractShape(width, height) {
    glInit();
}

Rectangle::Rectangle(int width, int height, int x, int y) : AbstractShape(width, height, x, y) {
    glInit();
}

std::vector<glm::vec2> Rectangle::points() const {
    glm::mat4 transform;
    if (m_updateMatrix) {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(m_position.x, m_position.y, 0.0f));
        transform = glm::rotate(transform, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(m_originalScale.x * m_scaleFactor.x, m_originalScale.y * m_scaleFactor.y, 1.0f));
    } else {
        transform = m_model;
    }

    std::vector<glm::vec2> res(4);
    for (size_t i = 0; i < 4; i++) {
        glm::vec4 vertex = transform * glm::vec4(vertices[i * 5], vertices[(i * 5) + 1], 0.0f, 1.0f);
        res[i] = glm::vec2(vertex.x, vertex.y);
    }

    return res;
}

void Rectangle::draw(const glm::mat4& lookAt, const glm::mat4& projection) {
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

    if (m_outlineThickness > 0.0f) {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        m_outlineShader.setMat4("model", m_outlineModel);
        m_outlineShader.setFloat("color", m_outlineColor.getNormalized());
        if (m_shader != m_outlineShader) {
            m_outlineShader.setMat4("view", lookAt);
            m_outlineShader.setMat4("projection", projection);
        }
        m_outlineShader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
    } else {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
    }
    glClear(GL_STENCIL_BUFFER_BIT);
}

my::Rectangle my::line(int x1, int y1, int x2, int y2) {
    double dist = glm::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    int center_x = (x2 + x1) / 2;
    int center_y = (y2 + y1) / 2;
    my::Rectangle res(std::lround(dist), 1);
    res.setPosition(center_x, center_y, true);
    double cos = (glm::abs(x2 - x1) / 2.0f) / (dist / 2.0f);
    res.setRotation(static_cast<float>(glm::degrees(glm::acos(cos))));
    return res;
}