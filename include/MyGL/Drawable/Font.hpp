#ifndef MY_FONT
#define MY_FONT

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../Header.hpp"

#include <vector>
#include <map>

namespace my
{
    /**
     * @brief Class for storing a font
    */
    class MYGL_EXPORT Font {
    private:
        /**
         * @brief Contains useful informations about a the representation of a glyph in a bitmap
        */
        struct Glyph {
            float texTop;
            float texBot;
            float texLeft;
            float texRight;
            int advance;
            unsigned int vao;
        };

        /**
         * @brief Class for storing a bitmap (a texture which contains all characters of a font)
        */
        class Bitmap {
        private:
            static const std::array<float, 12> rect_vertices;
            static const std::array<unsigned int, 4> rect_indices;

            unsigned int m_textureId = 0;
            size_t m_realSize = 0;
            std::vector<Glyph> m_alphabet{};

        public:
            /**
             * @brief Default constructor, doesn't initialize the Bitmap!!
            */
            Bitmap() = default;

            /**
             * @brief Creates a bitmap with characters of a given font size
             * @param size The (pixel) font size used to generate the glyphs
             * @param face The face provided by a my::Font
            */
            Bitmap(unsigned int size, FT_Face& face);

            /**
             * @brief Returns a reference to the list of Glyphs of this bitmap
             * @return a reference to a vector of Glyph
            */
            std::vector<Glyph>& getAlphabet();

            /**
             * @brief Indicates the size of the squares containing each character
             * @return The size in pixels of the square conatining a glyph in the texture
            */
            unsigned int getRealSize() const;

            /**
             * @brief
             * @return The bitmap's texture ID
            */
            unsigned int getTextureId() const;

            /**
             * @brief Compute the positions relative to the origin (0, 0) of all the characters of a string which
             *		  would be rendered using this bitmap
             * @param text The content of the string which will be rendered
             * @return A vector of positions, the nth position corresponding to the relative coordinates of the nth
             *		   character of the string
            */
            std::vector<std::pair<int, int>> computeGlyphsPos(const std::string& text) const;

        };// class Bitmap

        static FT_Library ftLib;
        static unsigned int instancesCount;
        FT_Face m_face;
        std::map<unsigned int, Bitmap> m_sizes;

        /**
         * @brief Creates a Bitmap for a given pixel character size
         * @param size The desired size
        */
        void createBitmap(unsigned int size);

        /**
         * @brief Checks if this font already has a bitmap conresponding to a size
         * @param size The size we want to check
         * @return true if a bitmap with this character size has already been created
        */
        bool hasSize(unsigned int size) const;

        /**
         * @brief Compute the positions relative to the origin (0, 0) of all the characters of a string which
         *		  would be rendered using this bitmap.
         *		  If the desired character size does not already have a bitmap, one is created
         * @param text text The content of the string which will be rendered
         * @param size the Strings character size
         * @return A vector of positions, the nth position corresponding to the relative coordinates of the nth
         *		   character of the string
        */
        std::vector<std::pair<int, int>> getCharsPos(const std::string& text, unsigned int size);

        /**
         * @brief Gives a reference to a list of Glyph
         * @param size The desired size
         * @return A reference to a vector conatining the all the Glyphs of the
         *		   Bitmap corresponding to this character size
        */
        std::vector<Glyph>& getAlphabet(unsigned int size);

        /**
         * @brief Indicates the scale to apply to a glyph of a bitmap to
         * @param size The desired character size
         * @return The scale factor we would have to apply to a glyph to give it its real size
        */
        unsigned int getScale(unsigned int size);

        /**
         * @brief Retrieves the texture ID of a bitmap
         * @param size The desired character size
         * @return The texture ID of the bitmap corresponding to the given character size
        */
        unsigned int getTextureId(unsigned int size);

    public:
        /**
         * @brief Default constructor, doesn't produce a usable font
        */
        Font();

        /**
         * @brief Creates a font from a font file
         *		  supported formats are the ones supported by freetype 2, including ttf, ttc, cff, woff, otf, otc
         *		  (at this point only ttf have been tested)
         * @param fontFilename The path to the font file
        */
        Font(const std::string& fontFilename);

        /**
         * @brief Loads a font
         * @param filename The path to the desired font
         * @return False if an error occured, true otherwise
        */
        bool load(const std::string& fontFilename);

        ~Font();

        friend class Text;
    };
}

#endif //MY_FONT