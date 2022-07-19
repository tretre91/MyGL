#include <MyGL/Drawable/Text.hpp>
#include <utf8.h>

constexpr const char* textVertexSource =
  "#version 330 core\n"
  "layout(location = 0) in vec3 aPos;"
  "layout(location = 1) in vec2 aTex;"
  "out vec2 texCoords;"
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 projection;"
  "void main() {"
  "    gl_Position = projection * view * model * vec4(aPos.x, -aPos.y, aPos.z, 1.0);"
  "    texCoords = vec2(aTex.x, 1.0 - aTex.y);"
  "}";

constexpr const char* textFragmentSource =
  "#version 330 core\n"
  "in vec2 texCoords;"
  "out vec4 FragColor;"
  "uniform sampler2D tex;"
  "uniform vec4 color;"
  "void main() {"
  "    FragColor = vec4(color.rgb, texture(tex, texCoords).r);"
  "}";

/**
 * @brief Convert a std::wstring to an utf8 encoded std::string
 */
std::string wstring_to_utf8(const std::wstring& str) {
    std::string result;
    if constexpr (sizeof(wchar_t) == 2) {
        utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
    } else {
        utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
    }
    return result;
}

/**
 * @brief Convert an utf8 encoded std::string to a std::wstring
 *
 * @param str
 * @return std::wstring
 */
std::wstring utf8_to_wstring(const std::string& str) {
    std::wstring result;
    if constexpr (sizeof(wchar_t) == 2) {
        utf8::utf8to16(str.begin(), str.end(), std::back_inserter(result));
    } else {
        utf8::utf8to32(str.begin(), str.end(), std::back_inserter(result));
    }
    return result;
}

namespace my
{
    ShaderProgram Text::textShader;
    Font Text::defaultFont;

    void Text::init() {
        if (!textShader.isUsable()) {
            textShader.addShaders(Shader(textVertexSource, Shader::Type::Vertex), Shader(textFragmentSource, Shader::Type::Fragment));
            textShader.link();
            textShader.setInt("tex", 3);
        }

        setColor(my::Color::black);
        m_texture = m_font.getStringTexture(m_text, m_size);
        m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
        m_isTextured = true;
        m_shader = textShader;
    }

    Text::Text() : m_font(defaultFont), m_size(0) {
        if (!textShader.isUsable()) {
            textShader.addShaders(Shader(textVertexSource, Shader::Type::Vertex), Shader(textFragmentSource, Shader::Type::Fragment));
            textShader.link();
            textShader.setInt("tex", 3);
        }
        m_shader = textShader;
    }

    my::Text::Text(const std::string& text, my::Font& font, unsigned int size) : m_text(text), m_font(font), m_size(size) {
        init();
    }

    my::Text::Text(const std::wstring& text, my::Font& font, unsigned int size) : m_text(wstring_to_utf8(text)), m_font(font), m_size(size) {
        init();
    }

    my::Text::Text(const std::u16string& text, my::Font& font, unsigned int size) : m_text(utf8::utf16to8(text)), m_font(font), m_size(size) {
        init();
    }

    my::Text::Text(const std::u32string& text, my::Font& font, unsigned int size) : m_text(utf8::utf32to8(text)), m_font(font), m_size(size) {
        init();
    }

    void Text::setContent(const std::string& text) {
        m_text = text;
        setTexture(m_font.getStringTexture(m_text, m_size));
        m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
        setPosition(getPosition(), true);
    }

    void Text::setContent(const std::wstring& text) {
        m_text = wstring_to_utf8(text);
        setTexture(m_font.getStringTexture(m_text, m_size));
        m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
        setPosition(getPosition(), true);
    }

    void Text::setContent(const std::u16string& text) {
        m_text = utf8::utf16to8(text);
        setTexture(m_font.getStringTexture(m_text, m_size));
        m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
        setPosition(getPosition(), true);
    }

    void Text::setContent(const std::u32string& text) {
        m_text = utf8::utf32to8(text);
        setTexture(m_font.getStringTexture(m_text, m_size));
        m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
        setPosition(getPosition(), true);
    }

    std::string Text::getString() const {
        return m_text;
    }

    std::wstring Text::getWString() const {
        return utf8_to_wstring(m_text);
    }

    std::u16string Text::getU16String() const {
        return utf8::utf8to16(m_text);
    }

    std::u32string Text::getU32String() const noexcept {
        return utf8::utf8to32(m_text);
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
} // namespace my
