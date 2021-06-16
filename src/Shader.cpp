#include <MyGL/Shader.hpp>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

namespace my
{
    std::unordered_map<unsigned int, int> Shader::instancesCount;

    Shader::Shader(const char* sourceCode, Type type) {
        loadFromString(sourceCode, type);
    }

    Shader::Shader(const Shader& shader) : m_shaderId(shader.m_shaderId), m_usable(shader.m_usable) {
        instancesCount[m_shaderId]++;
    }

    Shader::Shader(Shader&& shader) : m_shaderId(shader.m_shaderId), m_usable(shader.m_usable) {
        shader.m_shaderId = 0;
        shader.m_usable = false;
    }

    Shader::~Shader() {
        if (--instancesCount[m_shaderId] <= 0) {
            glDeleteShader(m_shaderId);
        }
    }

    Shader& Shader::operator=(const Shader& shader) {
        if (this != &shader) {
            if (--instancesCount[m_shaderId] <= 0) {
                glDeleteShader(m_shaderId);
            }
            m_shaderId = shader.m_shaderId;
            m_usable = shader.m_usable;
            instancesCount[m_shaderId]++;
        }
        return *this;
    }

    Shader& Shader::operator=(Shader&& shader) {
        if (this != &shader) {
            if (--instancesCount[m_shaderId] <= 0) {
                glDeleteShader(m_shaderId);
            }
            m_shaderId = shader.m_shaderId;
            m_usable = shader.m_usable;
            shader.m_shaderId = 0;
            shader.m_usable = false;
        }
        return *this;
    }

    bool Shader::loadFromFile(const std::string& filename, Type type) {
        std::ifstream sourceFile(filename);
        if (!sourceFile.is_open()) {
            std::cerr << "ERROR::SHADER: failed to open source file \"" << filename << "\"\n";
            m_usable = false;
            return m_usable;
        }

        std::ostringstream os;
        os << sourceFile.rdbuf();
        return loadFromString(os.str().c_str(), type);
    }

    bool Shader::loadFromString(const char* sourceCode, Type type) {
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

        m_shaderId = glCreateShader(shaderType);
        glShaderSource(m_shaderId, 1, &sourceCode, nullptr);
        glCompileShader(m_shaderId);

        int success;
        glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &success);
        m_usable = success;
        if (success == 0) {
            const size_t bufferSize = 512;
            char log[bufferSize];
            glGetShaderInfoLog(m_shaderId, bufferSize, nullptr, log);
            std::cerr << "ERROR::SHADER: failed to compile shader\n" << log;
        } else {
            instancesCount[m_shaderId]++;
        }
        return m_usable;
    }

    bool Shader::isUsable() const {
        return m_usable;
    }
} // namespace my