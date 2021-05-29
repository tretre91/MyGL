#include <MyGL/Shader.hpp>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
using namespace my;

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER: Failed to open file : " << filename << std::endl;
        return "";
    }
    std::ostringstream os;
    os << file.rdbuf();
    return os.str();
}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader, bool fromFile) {
    std::string vShaderCode;
    std::string fShaderCode;

    if (fromFile) {
        vShaderCode = readFile(vertexShader);
        fShaderCode = readFile(fragmentShader);
        if (vShaderCode.empty() || fShaderCode.empty()) {
            return;
        }
    } else {
        vShaderCode = vertexShader;
        fShaderCode = fragmentShader;
    }

    const char* vertexSource = vShaderCode.c_str();
    const char* fragmentSource = fShaderCode.c_str();

    m_isUsable = true;
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_shaderId = glCreateProgram();
    glAttachShader(m_shaderId, vertex);
    glAttachShader(m_shaderId, fragment);
    glLinkProgram(m_shaderId);

    glGetProgramiv(m_shaderId, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(m_shaderId, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    m_isUsable = true;

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

bool Shader::isUsable() const {
    return m_isUsable;
}

void Shader::use() const {
    glUseProgram(m_shaderId);
}

void Shader::setInt(const std::string& name, const glm::ivec1& value) const {
    use();
    glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()), value.x);
}

void Shader::setInt(const std::string& name, const glm::ivec2& value) const {
    use();
    glUniform2i(glGetUniformLocation(m_shaderId, name.c_str()), value.x, value.y);
}

void Shader::setInt(const std::string& name, const glm::ivec3& value) const {
    use();
    glUniform3i(glGetUniformLocation(m_shaderId, name.c_str()), value.x, value.y, value.z);
}

void Shader::setInt(const std::string& name, const glm::ivec4& value) const {
    use();
    glUniform4i(glGetUniformLocation(m_shaderId, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setUInt(const std::string& name, const glm::uvec1& value) const {
    use();
    glUniform1ui(glGetUniformLocation(m_shaderId, name.c_str()), value.x);
}

void Shader::setUInt(const std::string& name, const glm::uvec2& value) const {
    use();
    glUniform2ui(glGetUniformLocation(m_shaderId, name.c_str()), value.x, value.y);
}

void Shader::setUInt(const std::string& name, const glm::uvec3& value) const {
    use();
    glUniform3ui(glGetUniformLocation(m_shaderId, name.c_str()), value.x, value.y, value.z);
}

void Shader::setUInt(const std::string& name, const glm::uvec4& value) const {
    use();
    glUniform4ui(glGetUniformLocation(m_shaderId, name.c_str()), value.x, value.y, value.z, value.w);
}

void my::Shader::setFloat(const std::string& name, const glm::vec1& value) const {
    use();
    glUniform1f(glGetUniformLocation(m_shaderId, name.c_str()), value.x);
}

void my::Shader::setFloat(const std::string& name, const glm::vec2& value) const {
    use();
    glUniform2f(glGetUniformLocation(m_shaderId, name.c_str()), value.x, value.y);
}

void Shader::setFloat(const std::string& name, const glm::vec3& v) const {
    use();
    glUniform3f(glGetUniformLocation(m_shaderId, name.c_str()), v.x, v.y, v.z);
}

void Shader::setFloat(const std::string& name, const glm::vec4& value) const {
    use();
    glUniform4f(glGetUniformLocation(m_shaderId, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    use();
    glUniformMatrix4fv(glGetUniformLocation(m_shaderId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
