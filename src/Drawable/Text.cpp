#include <MyGL\Drawable\Text.hpp>
using namespace my;

const std::string Text::textVertexSource = "\
#version 330 core\n\
layout(location = 0) in vec3 aPos;\n\
layout(location = 1) in vec2 aTex\n;\
out vec2 texCoords;\n\
uniform mat4 model;\n\
uniform mat4 view;\n\
uniform mat4 projection;\n\
void main() {\n\
    gl_Position = projection * view * model * vec4(aPos, 1.0);\n\
    texCoords = vec2(aTex.x, 1.0 - aTex.y);\n\
}";

const std::string Text::textFragmentSource = "\
#version 330 core\n\
in vec2 texCoords;\n\
out vec4 FragColor;\n\
uniform sampler2D tex;\n\
uniform vec4 color;\n\
void main() {\n\
    FragColor = vec4(color.rgb, texture(tex, texCoords).r);\n\
}";

my::Shader Text::textShader = my::Shader();

my::Text::Text(const std::string& text, my::Font& font, unsigned int size) :
    m_text(text), m_font(font), m_size(size)
{
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

void Text::setTexture(const my::Texture& texture) {
    m_texture = texture;
}

void Text::setContent(const std::string& text) {
    m_text = text;
    setTexture(m_font.getStringTexture(m_text, m_size));
    m_originalScale = glm::vec2(m_texture.getWidth() / 2.0f, m_texture.getHeight() / 2.0f);
    setPosition(getPosition(), true);
}

std::string Text::getContent() const {
    return m_text;
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