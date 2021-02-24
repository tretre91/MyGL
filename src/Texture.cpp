#include <MyGL/Texture.hpp>
using namespace my;

Texture::Texture(const std::string& filename, GLenum format) : m_textureId(0) {
    if (!load(filename, format)) {
        std::cout << "Failed to load texture " << filename << std::endl;
    }
}

bool Texture::load(const std::string& filename, GLenum format) {
    if (m_textureId != 0) glDeleteTextures(1, &m_textureId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, numberOfChannels;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* data = stbi_load(filename.c_str(), &width, &height, &numberOfChannels, 0);

    if (data != NULL) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        m_width = width;
        m_height = width;
        stbi_image_free(data);
        return true;
    } else {
        m_width = 0;
        m_height = 0;
        return false;
    }
}

int Texture::getWidth() const {
    return m_width;
}

int Texture::getHeight() const {
    return m_height;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::setTextureWrapMethod(Axis axis, GLenum method){
    bind();
    if (axis == Axis::s || axis == Axis::x) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, method);
    else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, method);
}

void Texture::setBorderColor(int r, int g, int b, int alpha) {
    bind();
    float borderColor[] = {
        r / 255.0f,
        g / 255.0f,
        b / 255.0f,
        alpha / 255.0f
    };
    for(int i = 0; i < 4; i++) {
        if (borderColor[i] < 0.0f) borderColor[i] = 0.0f;
        else if (borderColor[i] > 1.0f) borderColor[i] = 1.0f;
    }
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void Texture::setBorderColor(const my::Color& color) {
    bind();
    const glm::vec4 nColor = color.getNormalized();
    float borderColor[] = { nColor.r, nColor.g, nColor.b, nColor.a };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}