#include <MyGL/ShaderProgram.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace my;

std::unordered_map<unsigned int, int> ShaderProgram::instancesCount;

ShaderProgram::ShaderProgram(const ShaderProgram& program) : m_programId(program.m_programId), m_usable(program.m_usable) {
    instancesCount[m_programId]++;
}

ShaderProgram::ShaderProgram(ShaderProgram&& program) : m_programId(program.m_programId), m_usable(program.m_usable) {
    program.m_programId = 0;
    program.m_usable = false;
}

ShaderProgram::~ShaderProgram() {
    if (--instancesCount[m_programId] <= 0) {
        glDeleteProgram(m_programId);
    }
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& program) {
    if (this != &program) {
        if (--instancesCount[m_programId] <= 0) {
            glDeleteProgram(m_programId);
        }
        m_programId = program.m_programId;
        m_usable = program.m_usable;
        instancesCount[m_programId]++;
    }
    return *this;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& program) {
    if (this != &program) {
        if (--instancesCount[m_programId] <= 0) {
            glDeleteProgram(m_programId);
        }
        m_programId = program.m_programId;
        m_usable = program.m_usable;
        program.m_programId = 0;
        program.m_usable = false;
    }
    return *this;
}

bool ShaderProgram::link() {
    glLinkProgram(m_programId);
    int success;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    m_usable = success;

    if (success == 0) {
        const size_t bufferSize = 512;
        char log[bufferSize];
        glGetProgramInfoLog(m_programId, bufferSize, nullptr, log);
        std::cerr << "ERROR::SHADER PROGRAM: link failed\n" << log;
    } else {
        instancesCount[m_programId]++;
    }

    return m_usable;
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

bool my::operator==(const ShaderProgram& lhs, const ShaderProgram& rhs) {
    return lhs.m_programId == rhs.m_programId;
}

bool my::operator!=(const ShaderProgram& lhs, const ShaderProgram& rhs) {
    return !(lhs == rhs);
}
