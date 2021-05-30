#include <MyGL/Shader.hpp>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
using namespace my;

Shader::Shader(const char* sourceCode, Type type) {
    loadFromString(sourceCode, type);
}

Shader::~Shader() {
    glDeleteShader(m_ShaderId);
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

    m_ShaderId = glCreateShader(shaderType);
    glShaderSource(m_ShaderId, 1, &sourceCode, nullptr);
    glCompileShader(m_ShaderId);

    int success;
    glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        const size_t bufferSize = 512;
        char log[bufferSize];
        glGetShaderInfoLog(m_ShaderId, bufferSize, nullptr, log);
        std::cerr << "ERROR::SHADER: failed to compile shader\n" << log;
    }

    m_usable = success;
    return m_usable;
}

bool Shader::isUsable() const {
    return m_usable;
}