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
        m_originalScale = glm::vec2(static_cast<float>(m_texture.getWidth()) / 2.0f, static_cast<float>(m_texture.getHeight()) / 2.0f);
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

    Text::Text(const std::string& text, my::Font& font, unsigned int size) : m_text(text), m_font(font), m_size(size) {
        init();
    }

    Text::~Text() {}

    const std::string& Text::getContent() const {
        return m_text;
    }

    void Text::setContent(const std::string& text) {
        m_text = text;
        setTexture(m_font.getStringTexture(m_text, m_size));
        m_originalScale = glm::vec2(static_cast<float>(m_texture.getWidth()) / 2.0f, static_cast<float>(m_texture.getHeight()) / 2.0f);
        setPosition(getPosition(), true);
    }


    void Text::setTexture(const my::Texture& texture) {
        m_texture = texture;
    }

    void Text::setFont(my::Font& font) {
        m_font = font;
        setTexture(m_font.getStringTexture(m_text, m_size));
        m_originalScale = glm::vec2(static_cast<float>(m_texture.getWidth()) / 2.0f, static_cast<float>(m_texture.getHeight()) / 2.0f);
        setPosition(getPosition(), true);
    }

    void Text::setFontSize(unsigned int size) {
        m_size = size;
        setTexture(m_font.getStringTexture(m_text, m_size));
        m_originalScale = glm::vec2(static_cast<float>(m_texture.getWidth()) / 2.0f, static_cast<float>(m_texture.getHeight()) / 2.0f);
        setPosition(getPosition(), true);
    }
} // namespace my
