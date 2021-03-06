#include <MyGL\Drawable\ConstText.hpp>
using namespace my;

const std::string ConstText::textVertexSource = "\
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

const std::string ConstText::textFragmentSource = "\
#version 330 core\n\
in vec2 texCoords;\n\
out vec4 FragColor;\n\
uniform sampler2D tex;\n\
uniform vec4 color;\n\
void main() {\n\
    FragColor = vec4(color.rgb, texture(tex, texCoords).r);\n\
}";

my::Shader ConstText::textShader = my::Shader();

my::ConstText::ConstText(const std::string& text, my::Font& font, unsigned int size) {
    if (!textShader.isUsable()) {
        textShader = my::Shader(textVertexSource, textFragmentSource, false);
        textShader.setInt("tex", 3);
    }
    my::Texture tex = font.getStringTexture(text, size);
    m_originalScale = glm::vec2(tex.getWidth() / 2.0f, tex.getHeight() / 2.0f);
    setTexture(tex);
    p_activeShader = &textShader;
}
