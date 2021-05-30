#include <MyGL/ShaderProgram.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace my;

bool my::ShaderProgram::link() {
    glLinkProgram(m_programId);
    int success;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if (success == 0) {
        const size_t bufferSize = 512;
        char log[bufferSize];
        glGetProgramInfoLog(m_programId, bufferSize, nullptr, log);
        std::cout << "ERROR::SHADER PROGRAM: link failed\n" << log;
    }

    m_usable = success;
    return m_usable;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_programId);
}

bool ShaderProgram::isUsable() const {
    return m_usable;
}

void ShaderProgram::use() const {
    glUseProgram(m_programId);
}

void ShaderProgram::setInt(const std::string& name, const glm::ivec1& value) const {
    use();
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value.x);
}

void ShaderProgram::setInt(const std::string& name, const glm::ivec2& value) const {
    use();
    glUniform2i(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y);
}

void ShaderProgram::setInt(const std::string& name, const glm::ivec3& value) const {
    use();
    glUniform3i(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z);
}

void ShaderProgram::setInt(const std::string& name, const glm::ivec4& value) const {
    use();
    glUniform4i(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z, value.w);
}

void ShaderProgram::setUInt(const std::string& name, const glm::uvec1& value) const {
    use();
    glUniform1ui(glGetUniformLocation(m_programId, name.c_str()), value.x);
}

void ShaderProgram::setUInt(const std::string& name, const glm::uvec2& value) const {
    use();
    glUniform2ui(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y);
}

void ShaderProgram::setUInt(const std::string& name, const glm::uvec3& value) const {
    use();
    glUniform3ui(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z);
}

void ShaderProgram::setUInt(const std::string& name, const glm::uvec4& value) const {
    use();
    glUniform4ui(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z, value.w);
}

void ShaderProgram::setFloat(const std::string& name, const glm::vec1& value) const {
    use();
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value.x);
}

void ShaderProgram::setFloat(const std::string& name, const glm::vec2& value) const {
    use();
    glUniform2f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y);
}

void ShaderProgram::setFloat(const std::string& name, const glm::vec3& v) const {
    use();
    glUniform3f(glGetUniformLocation(m_programId, name.c_str()), v.x, v.y, v.z);
}

void ShaderProgram::setFloat(const std::string& name, const glm::vec4& value) const {
    use();
    glUniform4f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z, value.w);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const {
    use();
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
