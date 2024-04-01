#include <MyGL/Drawable/Font.hpp>
#include <iostream>
#include <string>
#include <utf8.h>

namespace my
{
    FT_Library Font::ftLib = FT_Library();
    unsigned int Font::instancesCount = 0;

    void Font::addGlyph(FT_GlyphSlot& glyph, std::vector<uint8_t>& texture, FT_Pos x, FT_Pos y, FT_Pos width) {
        FT_Pos left = x + glyph->bitmap_left;
        FT_Pos top = y - glyph->bitmap_top;

        FT_Pos j_glyph = 0;
        for (FT_Pos j = top; j < top + glyph->bitmap.rows; j++, j_glyph++) {
            FT_Pos row = width * j;
            FT_Pos glyph_row = glyph->bitmap.width * (j_glyph);
            FT_Pos i_glyph = 0;
            for (FT_Pos i = left; i < left + glyph->bitmap.width; i++, i_glyph++) {
                texture[static_cast<std::size_t>(row + i)] = glyph->bitmap.buffer[glyph_row + i_glyph];
            }
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

        std::vector<std::pair<FT_Pos, FT_Pos>> charPos;
        charPos.reserve(static_cast<std::size_t>(utf8::distance(text.begin(), text.end())));

        FT_Pos textWidth = 0;
        FT_Pos trueSize = m_face->size->metrics.height >> 6;
        FT_Pos pen_x = 0;
        FT_Pos pen_y = -m_face->size->metrics.ascender >> 6;
        FT_UInt prevGlyphIndex = 0;
        FT_Vector kernDelta;
        const bool hasKerning = FT_HAS_KERNING(m_face);

        for (auto it = text.begin(); it != text.end();) {
            const char32_t codepoint = utf8::next(it, text.end());

            if (FT_Load_Char(m_face, codepoint, FT_LOAD_BITMAP_METRICS_ONLY) != 0) {
                std::cerr << "ERROR::FREETYPE: Couldn't load char" << utf8::utf32to8(std::u32string{codepoint, 1}) << '\n';
                continue;
            }

            charPos.emplace_back(pen_x, pen_y);
            if (codepoint == U'\n') {
                if (pen_x > textWidth) {
                    textWidth = pen_x;
                }
                pen_x = 0;
                pen_y -= trueSize;
            } else {
                if (hasKerning && prevGlyphIndex > 0 && m_face->glyph->glyph_index > 0) {
                    FT_Get_Kerning(m_face, prevGlyphIndex, m_face->glyph->glyph_index, FT_KERNING_DEFAULT, &kernDelta);
                    pen_x += kernDelta.x >> 6;
                }
                pen_x += m_face->glyph->advance.x >> 6;
            }
            prevGlyphIndex = m_face->glyph->glyph_index;
        }

        if (pen_x > textWidth) {
            textWidth = pen_x;
        }

        FT_Pos textHeight = -(pen_y + (m_face->size->metrics.descender >> 6)) + 10;

        std::vector<uint8_t> texture(static_cast<std::size_t>(textWidth * textHeight));
        size_t i = 0;
        for (auto it = text.begin(); it != text.end(); i++) {
            const char32_t codepoint = utf8::next(it, text.end());
            if (codepoint != U'\n') {
                if (FT_Load_Char(m_face, codepoint, FT_LOAD_RENDER) != 0) {
                    std::cerr << "ERROR::FREETYPE: Couldn't load char" << utf8::utf32to8(std::u32string{codepoint, 1}) << '\n';
                    continue;
                }
                addGlyph(m_face->glyph, texture, charPos[i].first, -charPos[i].second, textWidth);
            }
        }

        unsigned int texture_id;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(textWidth), static_cast<int>(textHeight), 0, GL_RED, GL_UNSIGNED_BYTE, texture.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return Texture(texture_id, static_cast<unsigned int>(textWidth), static_cast<unsigned int>(textHeight));
    }
} // namespace my
