#include <MyGL/Texture.hpp>
#include <array>
#include <iostream>

struct TextureDeleter {
    void operator()(unsigned int* textureId) {
        glDeleteTextures(1, textureId);
        delete textureId;
    }
};

namespace my
{
    Texture::Texture(const std::string& filename) {
        if (!load(filename)) {
            std::cerr << "ERROR:TEXTURE: Failed to load texture " << filename << '\n';
        }
    }

    Texture::Texture(const Image& image) {
        if (!load(image)) {
            std::cerr << "ERROR::TEXTURE: Failed to create texture\n";
        }
    }

    Texture::Texture(unsigned int textureId, unsigned int width, unsigned int height) :
      p_textureId(textureId != 0 ? new unsigned int{textureId} : nullptr, TextureDeleter()), m_width(width), m_height(height) {}

    void Texture::create(const Image& image) {
        unsigned int textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLenum format = GL_RGBA;
        switch (image.getChannels()) {
        case 1:
            format = GL_RED;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            break;

        case 2:
            format = GL_RG;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
            break;

        case 3:
            format = GL_RGB;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            break;

        case 4:
            format = GL_RGBA;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            break;
        }

        p_textureId.reset(new unsigned int{textureId}, TextureDeleter());
        m_width = static_cast<unsigned int>(image.getWidth());
        m_height = static_cast<unsigned int>(image.getHeight());
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    bool Texture::load(const std::string& filename) {
        const Image image(filename, true, 0);
        if (image.isUsable()) {
            create(image);
            return true;
        }
        return false;
    }

    bool Texture::load(const Image& image) {
        if (image.isUsable()) {
            create(Image(image.data(), image.getWidth(), image.getHeight(), image.getChannels(), true));
            return true;
        }
        return false;
    }

    unsigned int Texture::getId() const noexcept {
        return p_textureId != nullptr ? *p_textureId : 0;
    }

    unsigned int Texture::getWidth() const noexcept {
        return m_width;
    }

    unsigned int Texture::getHeight() const noexcept {
        return m_height;
    }

    void Texture::bind() const {
        glBindTexture(GL_TEXTURE_2D, p_textureId != nullptr ? *p_textureId : 0);
    }

    void Texture::setTextureWrapMethod(Axis axis, GLenum method) {
        bind();
        if (axis == Axis::s || axis == Axis::x) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, method);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, method);
        }
    }

    void Texture::setBorderColor(int r, int g, int b, int alpha) {
        setBorderColor(Color(r, g, b, alpha));
    }

    void Texture::setBorderColor(const Color& color) {
        bind();
        const glm::vec4 nColor = color.getNormalized();
        const std::array<float, 4> borderColor{nColor.r, nColor.g, nColor.b, nColor.a};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());
    }
} // namespace my