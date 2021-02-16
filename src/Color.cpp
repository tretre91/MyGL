#include <MyGL/Color.hpp>
using namespace my;

const Color Color::white = Color(255, 255, 255);
const Color Color::black = Color(0, 0, 0);
const Color Color::red = Color(255, 0, 0);
const Color Color::green = Color(0, 255, 0);
const Color Color::blue = Color(0, 0, 255);

Color::Color() : Color(Color::white) {
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) : r(r), g(g), b(b), alpha(alpha) {
}

Color::Color(const std::string& hexColor, uint8_t alpha) {
    unsigned long n = std::stoul(hexColor, nullptr, 16);
    r = n >> 16;
    n &= 0b000000001111111111111111ul;
    g = n >> 8;
    n &= 0b000000000000000011111111ul;
    b = n;
    this->alpha = alpha;
}

Color& Color::operator=(const Color& color) {
    this->r = color.r;
    this->g = color.g;
    this->b = color.b;
    this->alpha = color.alpha;
    return *this;
}

bool my::operator==(const Color& color1, const Color& color2) {
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.alpha == color2.alpha;
}

bool my::operator!=(const Color& color1, const Color& color2) {
    return !(color1 == color2);
}