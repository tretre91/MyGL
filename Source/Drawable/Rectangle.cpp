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

Rectangle::Rectangle() : moveByCenter(false), AbstractShape() {
    glInit();
}

Rectangle::Rectangle(int width, int height) : moveByCenter(false), AbstractShape(width, height) {
    glInit();
}

Rectangle::Rectangle(int width, int height, int x, int y) : moveByCenter(false), AbstractShape(width, height, x, y) {
    glInit();
}

void Rectangle::positionByCenter(bool center) {
    moveByCenter = center;
}

std::vector<glm::vec2> Rectangle::points() const {
    glm::mat4 transform;
    if (updateMatrix) {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(position.x + originalScale.x * (!moveByCenter), position.y - originalScale.y * (!moveByCenter), 0.0f));
        transform = glm::rotate(transform, glm::radians((float)rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(originalScale.x * scaleFactor.x, originalScale.y * scaleFactor.y, 1.0f));
    }
    else {
        transform = model;
    }

    std::vector<glm::vec2> res;
    for (size_t i : {0, 3, 9, 6}) {
        glm::vec4 vertex = transform * glm::vec4(vertices[i], vertices[i + 1], 0.0f, 1.0f);
        res.push_back(glm::vec2(vertex.x, vertex.y));
    }

    return res;
}

void Rectangle::draw(const glm::mat4& lookAt, const glm::mat4& projection) {
    if (updateMatrix) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x + originalScale.x * (!moveByCenter), position.y - originalScale.y * (!moveByCenter), 0.0f));
        model = glm::rotate(model, glm::radians((float)rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(originalScale.x * scaleFactor.x, originalScale.y * scaleFactor.y, 1.0f));
        updateMatrix = false;
    }
    glActiveTexture(GL_TEXTURE3);
    texture.bind();
    activeShader->setMat4("model", glm::value_ptr(model));
    activeShader->setMat4("view", glm::value_ptr(lookAt));
    activeShader->setMat4("projection", glm::value_ptr(projection));
    activeShader->setFloat("color", color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha / 255.0f);
    activeShader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
}