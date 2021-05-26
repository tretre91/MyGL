#include <MyGL/Color.hpp>
using namespace my;

const Color Color::white = Color(255, 255, 255);
const Color Color::black = Color(0, 0, 0);
const Color Color::red = Color(255, 0, 0);
const Color Color::green = Color(0, 255, 0);
const Color Color::blue = Color(0, 0, 255);

Color::Color() : Color(Color::white) {}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) : r(r), g(g), b(b), alpha(alpha), normalized(r / 255.0f, g / 255.0f, b / 255.0f, alpha / 255.0f) {}

Color::Color(const std::string& hexColor, uint8_t alpha) : normalized{} {
    unsigned long n = std::stoul(hexColor, nullptr, 16);
    r = static_cast<uint8_t>(n >> 16);
    n &= 0b000000001111111111111111ul;
    g = static_cast<uint8_t>(n >> 8);
    n &= 0b000000000000000011111111ul;
    b = static_cast<uint8_t>(n);
    this->alpha = alpha;

    normalized.r = r / 255.0f;
    normalized.g = g / 255.0f;
    normalized.b = b / 255.0f;
    normalized.a = alpha / 255.0f;
}

glm::ivec4 Color::get() const {
    return glm::ivec4(r, g, b, alpha);
}

glm::vec4 Color::getNormalized() const {
    return normalized;
}

int Color::getRed() const {
    return r;
}

int Color::getGreen() const {
    return g;
}

int Color::getBlue() const {
    return b;
}

int Color::getAlpha() const {
    return alpha;
}

void Color::setRed(uint8_t red) {
    r = red;
    normalized.r = r / 255.0f;
}

void Color::setGreen(uint8_t green) {
    g = green;
    normalized.g = g / 255.0f;
}

void Color::setBlue(uint8_t blue) {
    b = blue;
    normalized.b = b / 255.0f;
}

void Color::setAlpha(uint8_t alpha) {
    this->alpha = alpha;
    normalized.a = this->alpha / 255.0f;
}

bool my::operator==(const Color& color1, const Color& color2) {
    return color1.get() == color2.get();
}

bool my::operator!=(const Color& color1, const Color& color2) {
    return !(color1 == color2);
}