#include <MyGL/Drawable/Font.hpp>
using namespace my;

/**** my::Bitmap definition ****/

const std::array<float, 12> Font::Bitmap::rect_vertices = {
    -1.0f, -1.0f, 0.0f, //bottom left
    -1.0f,  1.0f, 0.0f, //top left
     1.0f, -1.0f, 0.0f, //bottom right
     1.0f,  1.0f, 0.0f //top right
};

const std::array<unsigned int, 4> Font::Bitmap::rect_indices = {
    0, 1, 2, 3
};

Font::Bitmap::Bitmap(unsigned int size, FT_Face& face) : mTextureId(0), realSize(0), mAlphabet(128), pFace(&face) {
    FT_Set_Pixel_Sizes(face, 0, size);

    realSize = static_cast<size_t>((face->size->metrics.ascender - face->size->metrics.descender) >> 6);
    const size_t baseline_pos = face->size->metrics.ascender >> 6;
    std::vector<uint8_t> bitmap(realSize * realSize * 128);
    std::fill(bitmap.begin(), bitmap.end(), 0);

    const size_t width = 16;
    const size_t height = 8;
    const size_t realWidth = width * realSize;
    const size_t realHeight = height * realSize;
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
            std::cout << "ERROR::FREETYPE: failed to load char \'" << c << "\'(" << (int)c << ")" << std::endl;
            continue;
        }
        uint8_t* charData = face->glyph->bitmap.buffer;

        size_t pixelCurrentHeight = currentHeight * realSize;
        size_t pixelCurrentWidth = currentWidth * realSize;
        size_t startHeight = pixelCurrentHeight + (baseline_pos - face->glyph->bitmap_top);
        size_t startWidth = pixelCurrentWidth + face->glyph->bitmap_left;

        size_t j_bis = 0;
        for (size_t j = startHeight; j < startHeight + face->glyph->bitmap.rows; j++) {
            size_t i_bis = 0;
            for (size_t i = startWidth; i < startWidth + face->glyph->bitmap.width; i++) {
                bitmap[i + j * realWidth] = charData[i_bis + j_bis * face->glyph->bitmap.width];
                i_bis++;
            }
            j_bis++;
        }

        mAlphabet[c] = {
            pixelCurrentHeight / static_cast<float>(realHeight),
            (pixelCurrentHeight + realSize) / static_cast<float>(realHeight),
            pixelCurrentWidth / static_cast<float>(realWidth),
            (pixelCurrentWidth + realSize) / static_cast<float>(realWidth),
            static_cast<int>(face->glyph->advance.x >> 6),
            0
        };

        const std::array<float, 8> texCoords = {
            mAlphabet[c].texLeft, mAlphabet[c].texBot,
            mAlphabet[c].texLeft, mAlphabet[c].texTop,
            mAlphabet[c].texRight, mAlphabet[c].texBot,
            mAlphabet[c].texRight, mAlphabet[c].texTop
        };

        unsigned int textureVbo;

        glGenVertexArrays(1, &mAlphabet[c].vao);
        glGenBuffers(1, &textureVbo);

        glBindVertexArray(mAlphabet[c].vao);

        glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, textureVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
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
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, realWidth, realHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

std::vector<Font::Glyph>& my::Font::Bitmap::getAlphabet() {
    return mAlphabet;
}

unsigned int Font::Bitmap::getRealSize() const {
    return static_cast<unsigned int>(realSize);
}

unsigned int Font::Bitmap::getTextureId() const {
    return mTextureId;
}

std::vector<std::pair<int, int>> Font::Bitmap::computeGlyphsPos(const std::string& text) const {
    std::vector<std::pair<int, int>> res;
    res.reserve(text.size());

    int pen_x{}, pen_y{};
    for (char c : text) {
        res.push_back({ pen_x, pen_y });
        if (c != '\n') {
            pen_x += mAlphabet[c].advance;
        }
        else {
            pen_x = 0;
            pen_y -= realSize;
        }
    }

    return res;
}

/**** my::Font definition ****/
FT_Library Font::sFtLib = FT_Library();
unsigned int Font::sInstancesCount = 0;

Font::Font() : mFace(), mSizes() {}

Font::Font(const std::string& fontFilename) : mFace(), mSizes() {
    int error;
    if (sInstancesCount == 0) {
        error = FT_Init_FreeType(&sFtLib);
        if (error) {
            std::cout << "ERROR::Freetype : freetype initialization failed" << std::endl;
        }
    }

    error = FT_New_Face(sFtLib, fontFilename.c_str(), 0, &mFace);
    if (error) {
        std::cout << "ERROR::FREETYPE : Could not load font \"" << fontFilename << "\"" << std::endl;
    }
}

Font::~Font() {
    FT_Done_Face(mFace);
    sInstancesCount--;
    if (sInstancesCount == 0) FT_Done_FreeType(sFtLib);
}

void Font::createBitmap(unsigned int size) {
    mSizes.insert({ size, Bitmap(size, mFace) });
}

bool Font::hasSize(unsigned int size) const {
    return mSizes.find(size) != mSizes.end();
}

std::vector<std::pair<int, int>> Font::getCharsPos(const std::string& text, unsigned int size) {
    if (!hasSize(size)) createBitmap(size);
    return mSizes.find(size)->second.computeGlyphsPos(text);
}

std::vector<Font::Glyph>& Font::getAlphabet(unsigned int size) {
    if (!hasSize(size)) createBitmap(size);
    return mSizes.find(size)->second.getAlphabet();
}

unsigned int Font::getScale(unsigned int size) {
    if (!hasSize(size)) createBitmap(size);
    return mSizes.find(size)->second.getRealSize();
}

unsigned int Font::getTextureId(unsigned int size) {
    if (!hasSize(size)) createBitmap(size);
    return mSizes.find(size)->second.getTextureId();
}