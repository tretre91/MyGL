#include <MyGL/Drawable/Text.hpp>
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
    texCoords = aTex;\n\
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

Text::Text(const std::string& text, my::Font& font, unsigned int size) : m_text(text), m_size(size), m_fontScale(font.getScale(m_size)),
    m_bitmapTexId(font.getTextureId(m_size)), m_charPositions(font.getCharsPos(m_text, m_size)), m_alphabet(font.getAlphabet(m_size)), AbstractShape()
{
    if (!textShader.isUsable()) {
        textShader = my::Shader(Text::textVertexSource, Text::textFragmentSource, false);
        textShader.setInt("tex", 2);
    }
    m_originalScale = glm::vec2(m_fontScale, m_fontScale);
}

std::vector<glm::vec2> my::Text::points() const // TODO
{
    return std::vector<glm::vec2>();
}

void Text::draw(const glm::mat4& lookAt, const glm::mat4& projection) {
    textShader.setMat4("view", lookAt);
    textShader.setMat4("projection", projection);
    textShader.setFloat("color", m_color.getNormalized());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_bitmapTexId);

    for (size_t i = 0; i < m_text.size(); i++) {
        if (m_text[i] == '\n') continue;
        m_model = glm::translate(glm::mat4(1.0f), glm::vec3(m_charPositions[i].first + m_position.x, m_charPositions[i].second + m_position.y, 0.0f));
        m_model = glm::scale(m_model, glm::vec3(m_originalScale.x , m_originalScale.y, 1.0f));
        m_model = glm::rotate(m_model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        textShader.setMat4("model", m_model);

        glBindVertexArray(m_alphabet[m_text[i]].vao);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
    }
}