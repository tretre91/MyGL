#include <MyGL/Image.hpp>
#include <cstring>
#include <iostream>
#include <stb_image.h>

struct ImageFileDeleter {
    void operator()(uint8_t* data) const {
        stbi_image_free(data);
    }
};

namespace my
{
    Image::Image(const std::string& filename, bool flip, int desiredChannels) {
        load(filename, flip, desiredChannels);
    }

    Image::Image(const uint8_t* data, size_t width, size_t height, int numberOfChannels, bool flip) {
        load(data, width, height, numberOfChannels, flip);
    }

    bool Image::load(const std::string& filename, bool flip, int desiredChannels) {
        stbi_set_flip_vertically_on_load(flip);
        int width, height, channels;
        p_data.reset(stbi_load(filename.c_str(), &width, &height, &channels, desiredChannels), ImageFileDeleter());
        stbi_set_flip_vertically_on_load(false);

        if (p_data == nullptr) {
            std::cerr << "ERROR::IMAGE: failed to load image file " << filename << '\n';
            return false;
        }

        m_width = static_cast<size_t>(width);
        m_height = static_cast<size_t>(height);
        m_channels = static_cast<unsigned short>(desiredChannels == 0 ? channels : desiredChannels);

        return true;
    }

    bool Image::load(const uint8_t* data, size_t width, size_t height, int numberOfChannels, bool flip) {
        p_data.reset(); // TODO: Remove to keep image unmodified if the load fails ?
        if (data == nullptr) {
            std::cerr << "ERROR::IMAGE: buffer is null\n";
            return false;
        }
        if (numberOfChannels < 1 || numberOfChannels > 4) {
            std::cerr << "ERROR::IMAGE: invalid number of channels (" << numberOfChannels << ")\n";
            return false;
        }
        if (width == 0 || height == 0) {
            std::cerr << "ERROR::IMAGE: Failed to load image, height or width is 0\n";
            return false;
        }

        m_width = width;
        m_height = height;
        m_channels = static_cast<unsigned short>(numberOfChannels);
        uint8_t* _data = new uint8_t[m_width * m_height * m_channels];

        if (_data == nullptr) {
            std::cerr << "ERROR::IMAGE: Failed to allocate\n";
            return false;
        }

        const size_t scanline_width = m_width * m_channels;

        if (flip) {
            for (size_t i = 0, j = m_height - 1; i < m_height; i++, j--) {
                std::memcpy(_data + (i * scanline_width), data + (j * scanline_width), scanline_width);
            }
        } else {
            for (size_t i = 0, j = 0; i < m_height; i++, j++) {
                std::memcpy(_data + (i * scanline_width), data + (j * scanline_width), scanline_width);
            }
        }

        p_data.reset(_data, std::default_delete<uint8_t[]>());
        return true;
    }

    uint8_t* Image::data() const noexcept {
        return p_data.get();
    }

    bool Image::isUsable() const noexcept {
        return p_data != nullptr;
    }

    size_t Image::getWidth() const noexcept {
        return m_width;
    }

    size_t Image::getHeight() const noexcept {
        return m_height;
    }

    int Image::getChannels() const noexcept {
        return m_channels;
    }

} // namespace my
