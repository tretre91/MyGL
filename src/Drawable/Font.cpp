#include <MyGL/Drawable/Font.hpp>
using namespace my;

FT_Library Font::ftLib = FT_Library();
unsigned int Font::instancesCount = 0;

void Font::addGlyph(FT_GlyphSlot& glyph, std::vector<uint8_t>& texture, size_t x, size_t y, size_t width) {
    size_t j_glyph = 0;
    for (size_t j = y - glyph->bitmap_top; j < y - glyph->bitmap_top + glyph->bitmap.rows; j++) {
        size_t row = width * j;
        size_t row_glyph = glyph->bitmap.width * (j_glyph);
        size_t i_glyph = 0;
        for (size_t i = x; i < x + glyph->bitmap.width; i++) {
            texture[row + i] = glyph->bitmap.buffer[row_glyph + i_glyph];
            i_glyph++;
        }
        j_glyph++;
    }
}

Font::Font() : m_face() {
    if (instancesCount == 0) {
        if (FT_Init_FreeType(&ftLib)) {
            std::cerr << "ERROR::Freetype : freetype initialization failed" << std::endl;
        }
    }
    instancesCount++;
}

Font::Font(const std::string& fontFilename) : Font() {
    if (!load(fontFilename)) {
        std::cerr << "ERROR::FREETYPE : Could not load font \"" << fontFilename << "\"" << std::endl;
    }
}

bool Font::load(const std::string& fontFilename) {
    FT_Done_Face(m_face);
    return FT_New_Face(ftLib, fontFilename.c_str(), 0, &m_face) == 0;
}

Font::~Font() {
    FT_Done_Face(m_face);
    instancesCount--;
    if (instancesCount == 0) FT_Done_FreeType(ftLib);
}

my::Texture Font::getStringTexture(const std::string& text, unsigned int size) {
    FT_Set_Pixel_Sizes(m_face, 0, size);

    std::vector<std::pair<int, int>> charPos;
    charPos.reserve(text.size());

    size_t width = 0;
    size_t trueSize = m_face->size->metrics.height >> 6;
    int pen_x = 0, pen_y = -m_face->size->metrics.ascender >> 6;
    for (auto c : text) {
        FT_Load_Char(m_face, c, FT_LOAD_ADVANCE_ONLY);
        charPos.push_back({ pen_x, pen_y });
        if (c == '\n') {
            if (pen_x > width) width = pen_x;
            pen_x = 0;
            pen_y -= static_cast<int>(trueSize);
        } else {
            pen_x += m_face->glyph->advance.x >> 6;
        }
    }
    if (pen_x > width) width = pen_x;
    size_t height = -(pen_y + (m_face->size->metrics.descender >> 6));

    std::vector<uint8_t> texture(width * height);
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] != '\n') {
            FT_Load_Char(m_face, text[i], FT_LOAD_RENDER);
            addGlyph(m_face->glyph, texture, charPos[i].first, -charPos[i].second, width);
        }
    }

    unsigned int texture_id;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<unsigned>(width), static_cast<unsigned>(height), 0, GL_RED, GL_UNSIGNED_BYTE, texture.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return Texture(texture_id, static_cast<unsigned>(width), static_cast<unsigned>(height));
}
