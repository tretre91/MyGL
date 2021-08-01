#ifndef MYGL_IMAGE
#define MYGL_IMAGE

#include "mygl_export.h"
#include <memory>
#include <string>

namespace my
{
    /**
     * @brief A class for manipulating imges
     */
    class MYGL_EXPORT Image
    {
    private:
        std::shared_ptr<uint8_t> p_data;
        size_t m_width{};
        size_t m_height{};
        unsigned short m_channels{};

    public:
        /**
         * @brief Default constructor
         * This constructor does not produce a valid Image
         */
        Image() = default;

        /**
         * @brief Creates an Image from a file
         * @param filename The image's filename
         * @param flip Indicates if the image should be vertically flipped (useful for OpenGL textures)
         * @param desiredChannels The number of channels the loaded image should have
         */
        Image(const std::string& filename, bool flip = false, int desiredChannels = 0);

        /**
         * @brief Loads an image from an array of pixels
         * @param data A pointer to the array of bytes
         * @param width The image's width
         * @param height The image's heigth
         * @param numberOfChannels The number of channels in the image
         * @param flip Indicates if the image should be vertically flipped
         */
        Image(const uint8_t* data, size_t width, size_t height, int numberOfChannels, bool flip = false);

        ~Image() = default;

        /**
         * @brief Loads an image stored in a file
         * @param filename The image's filename
         * @param flip Indicates if the image should be vertically flipped (useful for OpenGL textures)
         * @param desiredChannels The number of channels the loaded image should have
         * @return true if the image was successfully loaded, false otherwise
         */
        bool load(const std::string& filename, bool flip = false, int desiredChannels = 0);

        /**
         * @brief Loads an image from an array of pixels
         * @param data A pointer to the array of bytes
         * @param width The image's width
         * @param height The image's heigth
         * @param numberOfChannels The number of channels in the image
         * @param flip Indicates if the image should be vertically flipped
         * @return true if the image was successfully loaded, false otherwise
         */
        bool load(const uint8_t* data, size_t width, size_t height, int numberOfChannels, bool flip = false);

        /**
         * @brief Tells whether the image is usable
         * @return true if the image can be used
         */
        bool isUsable() const noexcept;

        /**
         * @brief Returns a read only pointer to the pixel array
         * @return
         */
        const uint8_t* data() const noexcept;

        /**
         * @brief Indicates the image's width
         * @return The image's width
         */
        size_t getWidth() const noexcept;

        /**
         * @brief Indicates the image's height
         * @return The image's height
         */
        size_t getHeight() const noexcept;

        /**
         * @brief Indicates the number of channels in the image
         * @return The number of channels
         */
        int getChannels() const noexcept;
    };
} // namespace my

#endif // MYGL_IMAGE