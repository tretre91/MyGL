#include <MyGL/Color.hpp>

namespace my
{
    const Color Color::white = Color(255, 255, 255);
    const Color Color::black = Color(0, 0, 0);
    const Color Color::red = Color(255, 0, 0);
    const Color Color::green = Color(0, 255, 0);
    const Color Color::blue = Color(0, 0, 255);

    Color::Color() noexcept : r{255}, g{255}, b{255}, alpha{255}, normalized{1.0f, 1.0f, 1.0f, 1.0f} {}

    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) noexcept :
      r(r), g(g), b(b), alpha(alpha), normalized(r / 255.0f, g / 255.0f, b / 255.0f, alpha / 255.0f) {}

    Color::Color(const std::string& hexColor, uint8_t alpha) {
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

    glm::ivec4 Color::get() const noexcept {
        return glm::ivec4(r, g, b, alpha);
    }

    glm::vec4 Color::getNormalized() const noexcept {
        return normalized;
    }

    int Color::getRed() const noexcept {
        return r;
    }

    int Color::getGreen() const noexcept {
        return g;
    }

    int Color::getBlue() const noexcept {
        return b;
    }

    int Color::getAlpha() const noexcept {
        return alpha;
    }

    void Color::setRed(uint8_t red) noexcept {
        r = red;
        normalized.r = r / 255.0f;
    }

    void Color::setGreen(uint8_t green) noexcept {
        g = green;
        normalized.g = g / 255.0f;
    }

    void Color::setBlue(uint8_t blue) noexcept {
        b = blue;
        normalized.b = b / 255.0f;
    }

    void Color::setAlpha(uint8_t alpha) noexcept {
        this->alpha = alpha;
        normalized.a = this->alpha / 255.0f;
    }
} // namespace my

bool operator==(const my::Color& color1, const my::Color& color2) noexcept {
    return color1.get() == color2.get();
}

bool operator!=(const my::Color& color1, const my::Color& color2) noexcept {
    return !(color1 == color2);
}