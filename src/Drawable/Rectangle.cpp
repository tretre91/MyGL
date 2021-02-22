#include <MyGL/Drawable/Rectangle.hpp>
using namespace my;

unsigned int Rectangle::VAO = 0, Rectangle::VBO = 0, Rectangle::EBO = 0;

const std::array<float, 20> Rectangle::vertices = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,//bottom left
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,//top left
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,//bottom right
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f//top right
};

const std::array<unsigned int, 4> Rectangle::indices = {
    0, 1, 2, 3
};

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

Rectangle::Rectangle() : AbstractShape() {
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
    if (updateMatrix) {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
        transform = glm::rotate(transform, glm::radians((float)rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(originalScale.x * scaleFactor.x, originalScale.y * scaleFactor.y, 1.0f));
    }
    else {
        transform = model;
    }

    std::vector<glm::vec2> res(4);
    for(size_t i = 0; i < 4; i++) {
        glm::vec4 vertex = transform * glm::vec4(vertices[i * 5], vertices[(i * 5) + 1], 0.0f, 1.0f);
        res[i] = glm::vec2(vertex.x, vertex.y);
    }

    return res;
}

void Rectangle::draw(const glm::mat4& lookAt, const glm::mat4& projection) {
    if (updateMatrix) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
        model = glm::rotate(model, glm::radians(static_cast<float>(rotationAngle)), glm::vec3(0.0f, 0.0f, 1.0f));
        if (outlineThickness > 0) {
            outlineModel = glm::scale(model, glm::vec3(static_cast<float>(outlineThickness) + originalScale.x * scaleFactor.x, static_cast<float>(outlineThickness) + originalScale.y * scaleFactor.y, 1.0f));
        }
        model = glm::scale(model, glm::vec3(originalScale.x * scaleFactor.x, originalScale.y * scaleFactor.y, 1.0f));
        updateMatrix = false;
    }
    glActiveTexture(GL_TEXTURE3);
    texture.bind();
    activeShader->setMat4("model", model);
    activeShader->setMat4("view", lookAt);
    activeShader->setMat4("projection", projection);
    activeShader->setFloat("color", color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha / 255.0f);
    activeShader->use();

    if (outlineThickness > 0) {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
        
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        shader.setMat4("model", outlineModel);
        shader.setFloat("color", outlineColor.r / 255.0f, outlineColor.g / 255.0f, outlineColor.b / 255.0f, outlineColor.alpha / 255.0f);
        if (isTextured) {
            shader.setMat4("view", lookAt);
            shader.setMat4("projection", projection);
        }
        shader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
    } else {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
    }
}