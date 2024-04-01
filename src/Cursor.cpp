#include <MyGL/Cursor.hpp>
#include <MyGL/common.hpp>
#include <iostream>
#include <stb_image.h>

struct CursorDeleter {
    void operator()(GLFWcursor* cursor) {
        glfwDestroyCursor(cursor);
    }
};

namespace my
{
    const Cursor Cursor::arrow(glfwFunction(glfwCreateStandardCursor, GLFW_ARROW_CURSOR));
    const Cursor Cursor::IBeam(glfwFunction(glfwCreateStandardCursor, GLFW_IBEAM_CURSOR));
    const Cursor Cursor::crosshair(glfwFunction(glfwCreateStandardCursor, GLFW_CROSSHAIR_CURSOR));
    const Cursor Cursor::hand(glfwFunction(glfwCreateStandardCursor, GLFW_HAND_CURSOR));
    const Cursor Cursor::horizontalResize(glfwFunction(glfwCreateStandardCursor, GLFW_HRESIZE_CURSOR));
    const Cursor Cursor::verticalResize(glfwFunction(glfwCreateStandardCursor, GLFW_VRESIZE_CURSOR));

    Cursor::Cursor() noexcept : Cursor(Cursor::arrow) {}

    Cursor::Cursor(GLFWcursor* cursor) : p_cursor(cursor, CursorDeleter()) {}

    Cursor::Cursor(const Image& image, int xhot, int yhot) {
        if (!load(image, xhot, yhot)) {
            p_cursor = arrow.p_cursor;
        }
    }

    bool Cursor::load(const Image& image, int xhot, int yhot) {
        if (image.isUsable()) {
            GLFWimage cursor;
            cursor.width = static_cast<int>(image.getWidth());
            cursor.height = static_cast<int>(image.getHeight());
            cursor.pixels = image.data();
            p_cursor.reset(glfwCreateCursor(&cursor, xhot, cursor.height - 1 - yhot), CursorDeleter());
        } else {
            p_cursor.reset();
        }
        if (p_cursor == nullptr) {
            std::cerr << "ERROR::GLFW: Cursor creation failed\n";
            return false;
        }
        return true;
    }

    bool Cursor::isUsable() const noexcept {
        return p_cursor != nullptr;
    }

} // namespace my
