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

    Cursor::Cursor(const std::string& filename, int xhot, int yhot) {
        if (!load(filename, xhot, yhot)) {
            p_cursor = arrow.p_cursor;
        }
    }

    bool Cursor::load(const std::string& filename, int xhot, int yhot) {
        GLFWimage image;
        image.pixels = stbi_load(filename.c_str(), &image.width, &image.height, nullptr, 4);
        if (image.pixels == nullptr) {
            std::cerr << "ERROR::MYGL::Cursor: Failed to load image \"" << filename << "\"\n";
            return false;
        }

        p_cursor.reset(glfwCreateCursor(&image, xhot, image.height - 1 - yhot), CursorDeleter());
        stbi_image_free(image.pixels);

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