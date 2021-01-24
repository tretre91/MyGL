#include <MyGL/Texture.hpp>
using namespace my;

Texture::Texture(const std::string& filename, GLenum format) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, numberOfChannels;
	stbi_set_flip_vertically_on_load(true);
	uint8_t* data = stbi_load(filename.c_str(), &width, &height, &numberOfChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture " << filename << std::endl;
	}
	stbi_image_free(data);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::setTextureWrapMethod(Axis axis, GLenum method){
	bind();
	if (axis == Axis::s || axis == Axis::x) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, method);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, method);
}

void Texture::setBorderColor(int r, int g, int b, int alpha) {
	r = r > 255 ? 255 : (r >= 0) * r;
	g = g > 255 ? 255 : (g >= 0) * g;
	b = b > 255 ? 255 : (b >= 0) * b;
	alpha = alpha > 255 ? 255 : (alpha >= 0) * alpha;
	float borderColor[] = { r / 255.0f, g / 255.0f, b / 255.0f, alpha / 255.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}
