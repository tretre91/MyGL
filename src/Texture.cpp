#include <MyGL/Texture.hpp>
#include <array>
#include <iostream>

namespace my
{
    std::unordered_map<unsigned int, unsigned int> Texture::instances;

    Texture::Texture(const std::string& filename) {
        if (!load(filename)) {
            std::cerr << "Failed to load texture " << filename << '\n';
        }
    }

    Texture::Texture(unsigned int textureId, unsigned int width, unsigned int height) : m_textureId(textureId), m_width(width), m_height(height) {
        if (m_textureId != 0) {
            instances[m_textureId]++;
        }
    }

    Texture::~Texture() {
        if (m_textureId != 0 && --instances[m_textureId] == 0) {
            glDeleteTextures(1, &m_textureId);
            instances.erase(m_textureId);
        }
    }

    Texture::Texture(const Texture& tex) : m_textureId(tex.m_textureId), m_width(tex.m_width), m_height(tex.m_height) {
        if (m_textureId != 0) {
            instances[m_textureId]++;
        }
    }

    Texture::Texture(Texture&& tex) noexcept : m_width(tex.m_width), m_height(tex.m_height) {
        if (m_textureId != 0 && m_textureId != tex.m_textureId) {
            if (--instances[m_textureId] == 0) {
                glDeleteTextures(1, &m_textureId);
                instances.erase(m_textureId);
            }
        }
        m_textureId = tex.m_textureId;
        tex.m_textureId = 0;
    }

    Texture& Texture::operator=(const Texture& tex) {
        m_textureId = tex.m_textureId;
        m_width = tex.m_width;
        m_height = tex.m_height;
        if (m_textureId != 0) {
            instances[m_textureId]++;
        }
        return *this;
    }

    Texture& Texture::operator=(Texture&& tex) noexcept {
        m_width = tex.m_width;
        m_height = tex.m_height;
        if (m_textureId != 0 && m_textureId != tex.m_textureId) {
            if (--instances[m_textureId] == 0) {
                glDeleteTextures(1, &m_textureId);
                instances.erase(m_textureId);
            }
        }
        m_textureId = tex.m_textureId;
        tex.m_textureId = 0;
        return *this;
    }

    bool Texture::load(const std::string& filename) {
        if (m_textureId != 0 && --instances[m_textureId] == 0) {
            glDeleteTextures(1, &m_textureId);
            instances.erase(m_textureId);
        }

        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, numberOfChannels;
        stbi_set_flip_vertically_on_load(true);
        uint8_t* data = stbi_load(filename.c_str(), &width, &height, &numberOfChannels, 0);

        if (data == nullptr) {
            glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
            m_width = 0;
            m_height = 0;
            return false;
        }

        GLenum format = GL_RGBA;
        switch (numberOfChannels) {
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

        m_width = width;
        m_height = height;
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        instances.insert({m_textureId, 1});
        stbi_image_free(data);
        return true;
    }

    unsigned int Texture::getId() const {
        return m_textureId;
    }

    unsigned int Texture::getWidth() const {
        return m_width;
    }

    unsigned int Texture::getHeight() const {
        return m_height;
    }

    void Texture::bind() const {
        glBindTexture(GL_TEXTURE_2D, m_textureId);
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