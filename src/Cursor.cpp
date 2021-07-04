#include <MyGL/Cursor.hpp>
#include <MyGL/common.hpp>
#include <iostream>
#include <stb_image.h>

namespace my
{
    std::unordered_map<GLFWcursor*, int> Cursor::instancesCount;
    const Cursor Cursor::arrow(glfwFunction(glfwCreateStandardCursor, GLFW_ARROW_CURSOR));
    const Cursor Cursor::IBeam(glfwFunction(glfwCreateStandardCursor, GLFW_IBEAM_CURSOR));
    const Cursor Cursor::crosshair(glfwFunction(glfwCreateStandardCursor, GLFW_CROSSHAIR_CURSOR));
    const Cursor Cursor::hand(glfwFunction(glfwCreateStandardCursor, GLFW_HAND_CURSOR));
    const Cursor Cursor::horizontalResize(glfwFunction(glfwCreateStandardCursor, GLFW_HRESIZE_CURSOR));
    const Cursor Cursor::verticalResize(glfwFunction(glfwCreateStandardCursor, GLFW_VRESIZE_CURSOR));

    Cursor::Cursor() : Cursor(Cursor::arrow) {}

    Cursor::Cursor(GLFWcursor* cursor) : p_cursor(cursor) {
        instancesCount[p_cursor]++;
    }

    Cursor::Cursor(const std::string& filename, int xhot, int yhot) : p_cursor(nullptr) {
        if (!load(filename, xhot, yhot)) {
            p_cursor = arrow.p_cursor;
            instancesCount[p_cursor]++;
        }
    }

    Cursor::Cursor(const Cursor& cursor) : p_cursor(cursor.p_cursor) {
        instancesCount[p_cursor]++;
    }

    Cursor::Cursor(Cursor&& cursor) : p_cursor(cursor.p_cursor) {
        cursor.p_cursor = nullptr;
    }

    Cursor::~Cursor() {
        if (p_cursor != nullptr && --instancesCount[p_cursor] == 0) {
            glfwDestroyCursor(p_cursor);
        }
    }

    Cursor& Cursor::operator=(const Cursor& cursor) {
        p_cursor = cursor.p_cursor;
        instancesCount[p_cursor]++;
        return *this;
    }

    Cursor& Cursor::operator=(Cursor&& cursor) {
        p_cursor = cursor.p_cursor;
        cursor.p_cursor = nullptr;
        return *this;
    }

    bool Cursor::load(const std::string& filename, int xhot, int yhot) {
        GLFWimage image;
        image.pixels = stbi_load(filename.c_str(), &image.width, &image.height, nullptr, 4);
        if (image.pixels == nullptr) {
            std::cerr << "ERROR::MYGL::Cursor: Failed to load image \"" << filename << "\"\n";
            return false;
        }

        if (p_cursor != nullptr && --instancesCount[p_cursor] == 0) {
            glfwDestroyCursor(p_cursor);
        }
        p_cursor = glfwCreateCursor(&image, xhot, yhot);
        stbi_image_free(image.pixels);

        if (p_cursor == nullptr) {
            std::cerr << "ERROR::GLFW: Cursor creation failed\n";
            return false;
        }
        instancesCount[p_cursor]++;
        return true;
    }

    bool Cursor::isUsable() const {
        return p_cursor != nullptr;
    }

} // namespace my