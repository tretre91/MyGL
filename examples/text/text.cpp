#include <MyGL/mygl.hpp>
#include <string>

int main() {
    my::Window window(800, 600, "Text");
    window.setFramerate(60);

    my::Font open_sans("@RESOURCES_DIR@/OpenSans-Regular.ttf");
    my::Font math_font("@RESOURCES_DIR@/latinmodern-math.otf");
    my::Font jp_font("@RESOURCES_DIR@/NotoSansJP-Regular.otf");

    const std::string lorem_ipsum_paragraph =
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod\n"
      "tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam,\n"
      "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo\n"
      "consequat.Duis aute irure dolor in reprehenderit in voluptate velit esse\n"
      "cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat cupidatat\n"
      "non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    my::Text lorem_ipsum(lorem_ipsum_paragraph, open_sans, 20);
    lorem_ipsum.setColor(my::Color::white);
    lorem_ipsum.setPosition(window.getSize() / 2, true);

    std::wstring greek_lowercase;
    for (wchar_t c = L'\u03B1'; c < L'\u03CA'; c++) {
        greek_lowercase += c;
    }
    my::Text greek_alphabet(greek_lowercase, open_sans, 30);
    greek_alphabet.setColor(my::Color::red);
    greek_alphabet.setPosition(50, 500);

    my::Text math_formula(U"\u2200\U0001D465 \u2208 \u211D\u2217, 1/\U0001D465 is defined", math_font, 30);
    math_formula.setColor(my::Color::green);
    math_formula.rotate(-20);
    math_formula.setPosition(600, 450, true);

    std::u32string hiraganas;
    for (char32_t c = U'\u3041'; c < U'\u305F'; c++) {
        if (c % 16 == 0) {
            hiraganas += U'\n';
        }
        hiraganas += c;
    }
    my::Text some_hiraganas(hiraganas, jp_font, 30);
    some_hiraganas.setColor(my::Color::white);
    some_hiraganas.setPosition(window.getSize().x / 2, 75, true);
    some_hiraganas.setOutlineThickness(2);
    some_hiraganas.setOutlineColor(202, 81, 0);

    const my::Color clear_color(50, 50, 50);

    my::Event e;

    while (window.isRunning()) {
        while (window.pollEvent(e)) {
            switch (e.type) {
            case my::EventType::windowShouldClose:
                window.close();
                break;

            case my::EventType::keyPressed:
                if (e.keyCode == my::Key::escape)
                    window.close();
                break;

            default:
                break;
            }
        }

        window.clear(clear_color);

        window.draw(lorem_ipsum);
        window.draw(greek_alphabet);
        window.draw(math_formula);
        window.draw(some_hiraganas);

        window.display();
    }

    return 0;
}