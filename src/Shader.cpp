#include <MyGL/Shader.hpp>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

struct ShaderDeleter {
    void operator()(unsigned int* shaderId) {
        if (shaderId != nullptr) {
            glDeleteShader(*shaderId);
            delete shaderId;
        }
    }
};

namespace my
{
    Shader::Shader(const std::string& sourceCode, Type type) {
        loadFromString(sourceCode, type);
    }

    bool Shader::loadFromFile(const std::string& filename, Type type) {
        std::ifstream sourceFile(filename);
        if (!sourceFile.is_open()) {
            std::cerr << "ERROR::SHADER: failed to open source file \"" << filename << "\"\n";
            p_shaderId.reset();
            return false;
        }

        std::ostringstream os;
        os << sourceFile.rdbuf();
        sourceFile.close();
        return loadFromString(os.str(), type);
    }

    bool Shader::loadFromString(const std::string& sourceCode, Type type) {
        GLenum shaderType;
        switch (type) {
        case Type::Vertex:
            shaderType = GL_VERTEX_SHADER;
            break;
        case Type::Fragment:
            shaderType = GL_FRAGMENT_SHADER;
            break;
        case Type::Geometry:
            shaderType = GL_GEOMETRY_SHADER;
            break;
        }

        const char* code = sourceCode.c_str();
        unsigned int shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &code, nullptr);
        glCompileShader(shaderId);

        int success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

        if (success == 0) {
            const size_t bufferSize = 512;
            char log[bufferSize];
            glGetShaderInfoLog(shaderId, bufferSize, nullptr, log);
            std::cerr << "ERROR::SHADER: failed to compile shader\n" << log;
            p_shaderId.reset();
            return false;
        }

        p_shaderId.reset(new unsigned int{shaderId}, ShaderDeleter());
        return true;
    }

    bool Shader::isUsable() const noexcept {
        return p_shaderId != nullptr;
    }
} // namespace my