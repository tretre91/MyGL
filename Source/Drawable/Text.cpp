#include <MyGL/Drawable/Text.hpp>
using namespace my;

const std::string Text::textVertexSource =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec2 aTex\n;"

"out vec2 texCoords;\n"

"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main() {\n"
"	gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"	texCoords = aTex;\n"
"}";

const std::string Text::textFragmentSource =
"#version 330 core\n"
"in vec2 texCoords;\n"
"out vec4 color;\n"

"uniform sampler2D tex;\n"
"uniform vec3 textColor;\n"

"void main() {\n"
"	color = vec4(textColor, texture(tex, texCoords).r);\n"
"}";

my::Shader Text::textShader = my::Shader();

Text::Text(const std::string& text, my::Font& font, unsigned int size) : mText(text), mSize(size), mFontScale(font.getScale(mSize)),
mBitmapTexId(font.getTextureId(mSize)), charPositions(font.getCharsPos(mText, mSize)), alphabet(font.getAlphabet(mSize)), AbstractShape() {
	if (!textShader.isUsable()) textShader = my::Shader(true, Text::textVertexSource, Text::textFragmentSource);
	textShader.setInt("tex", 2);
}

std::vector<glm::vec2> my::Text::points() const // TODO
{
	return std::vector<glm::vec2>();
}

void Text::draw(const glm::mat4& lookAt, const glm::mat4& projection) {
	textShader.setMat4("view", glm::value_ptr(lookAt));
	textShader.setMat4("projection", glm::value_ptr(projection));
	textShader.setFloat("color", color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mBitmapTexId);

	for (size_t i = 0; i < mText.size(); i++) {
		if (mText[i] == '\n') continue;
		model = glm::translate(glm::mat4(1.0f), glm::vec3(charPositions[i].first + position.x, charPositions[i].second + position.y, 0.0f));
		model = glm::scale(model, glm::vec3(mFontScale / 2.0f, mFontScale / 2.0f, 1.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		textShader.setMat4("model", glm::value_ptr(model));

		glBindVertexArray(alphabet[mText[i]].vao);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
	}
}