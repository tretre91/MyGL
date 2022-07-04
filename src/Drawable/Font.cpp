#include <MyGL/Drawable/Font.hpp>
#include <iostream>
#include <utf8.h>

namespace my
{
    FT_Library Font::ftLib = FT_Library();
    unsigned int Font::instancesCount = 0;

    void Font::addGlyph(FT_GlyphSlot& glyph, std::vector<uint8_t>& texture, size_t x, size_t y, size_t width) {
        size_t j_glyph = 0;
        size_t left = x + glyph->bitmap_left;
        size_t top = y - glyph->bitmap_top;
        for (size_t j = top; j < top + glyph->bitmap.rows; j++) {
            size_t row = width * j;
            size_t row_glyph = glyph->bitmap.width * (j_glyph);
            size_t i_glyph = 0;
            for (size_t i = left; i < left + glyph->bitmap.width; i++) {
                texture[row + i] = glyph->bitmap.buffer[row_glyph + i_glyph];
                i_glyph++;
            }
            j_glyph++;
        }
    }

    Font::Font() : m_face() {
        if (instancesCount == 0) {
            if (FT_Init_FreeType(&ftLib) != 0) {
                std::cerr << "ERROR::Freetype : freetype initialization failed\n";
            }
        }
        instancesCount++;
    }

    Font::Font(const std::string& fontFilename) : Font() {
        if (!load(fontFilename)) {
            std::cerr << "ERROR::FREETYPE: Could not load font \"" << fontFilename << "\"\n";
        }
    }

    bool Font::load(const std::string& fontFilename) {
        FT_Done_Face(m_face);
        return FT_New_Face(ftLib, fontFilename.c_str(), 0, &m_face) == 0 && FT_Select_Charmap(m_face, FT_ENCODING_UNICODE) == 0;
    }

    Font::~Font() {
        FT_Done_Face(m_face);
        instancesCount--;
        if (instancesCount == 0) {
            FT_Done_FreeType(ftLib);
        }
    }

    Texture Font::getStringTexture(const std::string& text, unsigned int size) {
        FT_Set_Pixel_Sizes(m_face, 0, size);

        std::vector<std::pair<int, int>> charPos;
        charPos.reserve(utf8::distance(text.begin(), text.end()));

        size_t width = 0;
        size_t trueSize = m_face->size->metrics.height >> 6;
        int pen_x = 0;
        int pen_y = -m_face->size->metrics.ascender >> 6;
        FT_UInt prevGlyphIndex = 0;
        FT_Vector kernDelta;
        const bool hasKerning = FT_HAS_KERNING(m_face);

        for (auto it = text.begin(); it != text.end();) {
            const char32_t c = utf8::next(it, text.end());

            if (FT_Load_Char(m_face, c, FT_LOAD_BITMAP_METRICS_ONLY) != 0) {
                std::wcerr << "ERROR::FREETYPE: Couldn't load char" << c << '\n';
                continue;
            }

            charPos.emplace_back(std::pair<int, int>{pen_x, pen_y});
            if (c == U'\n') {
                if (pen_x > width) {
                    width = pen_x;
                }
                pen_x = 0;
                pen_y -= static_cast<int>(trueSize);
            } else {
                if (hasKerning && prevGlyphIndex > 0 && m_face->glyph->glyph_index > 0) {
                    FT_Get_Kerning(m_face, prevGlyphIndex, m_face->glyph->glyph_index, FT_KERNING_DEFAULT, &kernDelta);
                    pen_x += kernDelta.x >> 6;
                }
                pen_x += m_face->glyph->advance.x >> 6;
            }
            prevGlyphIndex = m_face->glyph->glyph_index;
        }

        if (pen_x > width) {
            width = pen_x;
        }
        size_t height = static_cast<size_t>(-(pen_y + (m_face->size->metrics.descender >> 6))) + 10;

        std::vector<uint8_t> texture(width * height);
        size_t i = 0;
        for (auto it = text.begin(); it != text.end(); i++) {
            const char32_t c = utf8::next(it, text.end());
            if (c != U'\n') {
                if (FT_Load_Char(m_face, c, FT_LOAD_RENDER) != 0) {
                    std::wcerr << "ERROR::FREETYPE: Couldn't load char" << c << '\n';
                    continue;
                }
                addGlyph(m_face->glyph, texture, charPos[i].first, -charPos[i].second, width);
            }
        }

        unsigned int texture_id;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RED, static_cast<unsigned int>(width), static_cast<unsigned int>(height), 0, GL_RED, GL_UNSIGNED_BYTE, texture.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return Texture(texture_id, static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    }
} // namespace my