#include <MyGL/Drawable/Text.hpp>
using namespace my;

const std::string Text::textVertexSource =
  "#version 330 core\n"
  "layout(location = 0) in vec3 aPos;"
  "layout(location = 1) in vec2 aTex;"
  "out vec2 texCoords;"
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 projection;"
  "void main() {"
  "    gl_Position = projection * view * model * vec4(aPos, 1.0);"
  "    texCoords = vec2(aTex.x, 1.0 - aTex.y);"
  "}";

const std::string Text::textFragmentSource =
  "#version 330 core\n"
  "in vec2 texCoords;"
  "out vec4 FragColor;"
  "uniform sampler2D tex;"
  "uniform vec4 color;"
  "void main() {"
  "    FragColor = vec4(color.rgb, texture(tex, texCoords).r);"
  "}";

my::Shader Text::textShader = my::Shader();

my::Font Text::defaultFont = my::Font();

std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> Text::u8_u32conv{};
std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> Text::u16_u32conv{};
std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> Text::u8_wconv{};

Text::Text() : m_text(U""), m_font(defaultFont), m_size(0) {}

my::Text::Text(const std::string& text, my::Font& font, unsigned int size) : m_text(u8_u32conv.from_bytes(text)), m_font(font), m_size(size) {
    if (!textShader.isUsable()) {
        textShader = my::Shader(textVertexSource, textFragmentSource, false);
        textShader.setInt("tex", 3);
    }

    setColor(my::Color::black);
    setTexture(font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    m_isTextured = true;
    p_activeShader = &textShader;
}

my::Text::Text(const std::wstring& text, my::Font& font, unsigned int size) :
  m_text(u8_u32conv.from_bytes(u8_wconv.to_bytes(text))), m_font(font), m_size(size) {
    if (!textShader.isUsable()) {
        textShader = my::Shader(textVertexSource, textFragmentSource, false);
        textShader.setInt("tex", 3);
    }

    setColor(my::Color::black);
    setTexture(font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    m_isTextured = true;
    p_activeShader = &textShader;
}

my::Text::Text(const std::u16string& text, my::Font& font, unsigned int size) : m_font(font), m_size(size) {
    if (!textShader.isUsable()) {
        textShader = my::Shader(textVertexSource, textFragmentSource, false);
        textShader.setInt("tex", 3);
    }

    const char16_t* cText = text.c_str();
    m_text = u16_u32conv.from_bytes(reinterpret_cast<const char*>(cText), reinterpret_cast<const char*>(cText + text.size()));

    setColor(my::Color::black);
    setTexture(font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    m_isTextured = true;
    p_activeShader = &textShader;
}

my::Text::Text(const std::u32string& text, my::Font& font, unsigned int size) : m_text(text), m_font(font), m_size(size) {
    if (!textShader.isUsable()) {
        textShader = my::Shader(textVertexSource, textFragmentSource, false);
        textShader.setInt("tex", 3);
    }

    setColor(my::Color::black);
    setTexture(font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    m_isTextured = true;
    p_activeShader = &textShader;
}

void Text::setContent(const std::string& text) {
    m_text = u8_u32conv.from_bytes(text);
    setTexture(m_font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    setPosition(getPosition(), true);
}

void Text::setContent(const std::wstring& text) {
    m_text = u8_u32conv.from_bytes(u8_wconv.to_bytes(text));
    setTexture(m_font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    setPosition(getPosition(), true);
}

void Text::setContent(const std::u16string& text) {
    const char16_t* cText = text.c_str();
    m_text = u16_u32conv.from_bytes(reinterpret_cast<const char*>(cText), reinterpret_cast<const char*>(cText + text.size()));
    setTexture(m_font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    setPosition(getPosition(), true);
}

void Text::setContent(const std::u32string& text) {
    m_text = text;
    setTexture(m_font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    setPosition(getPosition(), true);
}

std::string Text::getString() const {
    return u8_u32conv.to_bytes(m_text);
}

std::wstring Text::getWString() const {
    return u8_wconv.from_bytes(u8_u32conv.to_bytes(m_text));
}

std::u16string Text::getU16String() const {
    return reinterpret_cast<const char16_t*>(u16_u32conv.to_bytes(m_text).c_str());
}

std::u32string Text::getU32String() const {
    return m_text;
}

void Text::setTexture(const my::Texture& texture) {
    m_texture = texture;
}

void Text::setFont(my::Font& font) {
    m_font = font;
    setTexture(m_font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    setPosition(getPosition(), true);
}

void Text::setFontSize(unsigned int size) {
    m_size = size;
    setTexture(m_font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    setPosition(getPosition(), true);
}