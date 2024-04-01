#ifndef MYGL_FONT
#define MYGL_FONT

#include "../mygl_export.h"

#include <ft2build.h>
#include <glad/gl.h>
#include FT_FREETYPE_H

#include "../Texture.hpp"

#include <string>
#include <vector>

namespace my
{
    /**
     * @brief Class for storing a font
     */
    class Font
    {
    private:
        static FT_Library ftLib;
        static unsigned int instancesCount;
        FT_Face m_face;

        /**
         * @brief Helper method for getStringTexture()
         * @param glyph A glyph slot containing the bitmap we want to copy (it
         *        should be loaded with the FT_LOAD_RENDER flag)
         * @param texture The vector we want to twrite to
         * @param x The x coord of the top left hand corner of the zone we wil copy the bitmap to
         * @param y The y coord of the top left hand corner of the zone we wil copy the bitmap to
         * @param width The final texture's width
         */
        static void addGlyph(FT_GlyphSlot& glyph, std::vector<uint8_t>& texture, FT_Pos x, FT_Pos y, FT_Pos width);

        /**
         * @brief Creates a texture containing a string
         * @param text The desired text
         * @param size The font size
         * @return A texture containing the specified text
         */
        Texture getStringTexture(const std::string& text, unsigned int size);

    public:
        /**
         * @brief Default constructor, doesn't produce a usable font
         */
        MYGL_EXPORT Font();

        /**
         * @brief Creates a font from a font file
         *
         * supported formats are the ones supported by freetype 2, including ttf, ttc, cff, woff, otf, otc
         * (at this point only ttf files have been tested)
         *
         * @param fontFilename The path to the font file
         */
        MYGL_EXPORT Font(const std::string& fontFilename);

        /**
         * @brief Loads a font
         * @param fontFilename The path to the desired font
         * @return False if an error occured, true otherwise
         */
        MYGL_EXPORT bool load(const std::string& fontFilename);

        MYGL_EXPORT ~Font();

        friend class Text;
    };

} // namespace my

#endif // MYGL_FONT
