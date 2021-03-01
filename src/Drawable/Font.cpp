#include <MyGL/Drawable/Font.hpp>
using namespace my;

///////////////////////////////////////////////////////////////////////////////
////////////////////////// my::Bitmap definition //////////////////////////////
///////////////////////////////////////////////////////////////////////////////

const std::array<float, 12> Font::Bitmap::rect_vertices = {
    -1.0f, -1.0f, 0.0f, //bottom left
    -1.0f,  1.0f, 0.0f, //top left
     1.0f, -1.0f, 0.0f, //bottom right
     1.0f,  1.0f, 0.0f //top right
};

const std::array<unsigned int, 4> Font::Bitmap::rect_indices = {
    0, 1, 2, 3
};

Font::Bitmap::Bitmap(unsigned int size, FT_Face& face) : m_textureId(0), m_realSize(0), m_alphabet(128) {
    FT_Set_Pixel_Sizes(face, 0, size);

    m_realSize = static_cast<size_t>((face->size->metrics.ascender - face->size->metrics.descender) >> 6);
    const size_t baseline_pos = face->size->metrics.ascender >> 6;
    std::vector<uint8_t> bitmap(m_realSize * m_realSize * 128);

    const size_t width = 16;
    const size_t height = 8;
    const size_t realWidth = width * m_realSize;
    const size_t realHeight = height * m_realSize;
    size_t currentWidth = 0;
    size_t currentHeight = 0;

    unsigned int vertexVbo, ebo;
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vertexVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices.data(), GL_STATIC_DRAW);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: failed to load char \'" << c << "\'(" << static_cast<int>(c) << ")" << std::endl;
            continue;
        }
        uint8_t* charData = face->glyph->bitmap.buffer;

        size_t pixelCurrentHeight = currentHeight * m_realSize;
        size_t pixelCurrentWidth = currentWidth * m_realSize;
        size_t startHeight = pixelCurrentHeight + (baseline_pos - face->glyph->bitmap_top);
        size_t startWidth = face->glyph->bitmap_left < 0 ? pixelCurrentWidth : pixelCurrentWidth + face->glyph->bitmap_left;

        size_t j_bis = 0;
        for (size_t j = startHeight; j < startHeight + face->glyph->bitmap.rows; j++) {
            size_t i_bis = 0;
            size_t bmp_row = j * realWidth;
            size_t glyph_row = j_bis * face->glyph->bitmap.width;
            for (size_t i = startWidth; i < startWidth + face->glyph->bitmap.width; i++) {
                bitmap[i + bmp_row] = charData[i_bis + glyph_row];
                i_bis++;
            }
            j_bis++;
        }

        m_alphabet[c] = {
            pixelCurrentHeight / static_cast<float>(realHeight),
            (pixelCurrentHeight + m_realSize) / static_cast<float>(realHeight),
            pixelCurrentWidth / static_cast<float>(realWidth),
            (pixelCurrentWidth + m_realSize) / static_cast<float>(realWidth),
            static_cast<int>(face->glyph->advance.x >> 6),
            0
        };

        const std::array<float, 8> texCoords = {
            m_alphabet[c].texLeft, m_alphabet[c].texBot,
            m_alphabet[c].texLeft, m_alphabet[c].texTop,
            m_alphabet[c].texRight, m_alphabet[c].texBot,
            m_alphabet[c].texRight, m_alphabet[c].texTop
        };

        unsigned int textureVbo;

        glGenVertexArrays(1, &m_alphabet[c].vao);
        glGenBuffers(1, &textureVbo);

        glBindVertexArray(m_alphabet[c].vao);

        glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        currentWidth++;
        if (currentWidth == width) {
            currentWidth = 0;
            currentHeight++;
        }
    }
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(realWidth), static_cast<int>(realHeight), 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

std::vector<Font::Glyph>& my::Font::Bitmap::getAlphabet() {
    return m_alphabet;
}

unsigned int Font::Bitmap::getRealSize() const {
    return static_cast<unsigned int>(m_realSize);
}

unsigned int Font::Bitmap::getTextureId() const {
    return m_textureId;
}

std::vector<std::pair<int, int>> Font::Bitmap::computeGlyphsPos(const std::string& text) const {
    std::vector<std::pair<int, int>> res;
    res.reserve(text.size());

    int pen_x{}, pen_y{};
    for (char c : text) {
        res.push_back({ pen_x, pen_y });
        if (c != '\n') {
            pen_x += m_alphabet[c].advance;
        }
        else {
            pen_x = 0;
            pen_y -= static_cast<int>(m_realSize);
        }
    }

    return res;
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////// my::Font definition //////////////////////////////
///////////////////////////////////////////////////////////////////////////////

FT_Library Font::ftLib = FT_Library();
unsigned int Font::instancesCount = 0;

Font::Font() : m_face(), m_sizes() {
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

void Font::createBitmap(unsigned int size) {
    m_sizes.insert({ size, Bitmap(size, m_face) });
}

bool Font::hasSize(unsigned int size) const {
    return m_sizes.find(size) != m_sizes.end();
}

std::vector<std::pair<int, int>> Font::getCharsPos(const std::string& text, unsigned int size) {
    if (!hasSize(size)) createBitmap(size);
    return m_sizes.find(size)->second.computeGlyphsPos(text);
}

std::vector<Font::Glyph>& Font::getAlphabet(unsigned int size) {
    if (!hasSize(size)) createBitmap(size);
    return m_sizes.find(size)->second.getAlphabet();
}

unsigned int Font::getScale(unsigned int size) {
    if (!hasSize(size)) createBitmap(size);
    return m_sizes.find(size)->second.getRealSize();
}

unsigned int Font::getTextureId(unsigned int size) {
    if (!hasSize(size)) createBitmap(size);
    return m_sizes.find(size)->second.getTextureId();
}