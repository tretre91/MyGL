#include <MyGL/ShaderProgram.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace my
{
    bool ShaderProgram::link() {
        if (p_programId != nullptr) {
            glLinkProgram(*p_programId);
            int success;
            glGetProgramiv(*p_programId, GL_LINK_STATUS, &success);
            if (success == 0) {
                const size_t bufferSize = 512;
                char log[bufferSize];
                glGetProgramInfoLog(*p_programId, bufferSize, nullptr, log);
                std::cerr << "ERROR::SHADER PROGRAM: link failed\n" << log;
                p_programId.reset();
                return false;
            }
        }
        return true;
    }

    bool ShaderProgram::isUsable() const noexcept {
        return p_programId != nullptr;
    }

    void ShaderProgram::use() const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
        }
    }

    void ShaderProgram::setInt(const std::string& name, const glm::ivec1& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform1i(glGetUniformLocation(*p_programId, name.c_str()), value.x);
        }
    }

    void ShaderProgram::setInt(const std::string& name, const glm::ivec2& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform2i(glGetUniformLocation(*p_programId, name.c_str()), value.x, value.y);
        }
    }

    void ShaderProgram::setInt(const std::string& name, const glm::ivec3& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform3i(glGetUniformLocation(*p_programId, name.c_str()), value.x, value.y, value.z);
        }
    }

    void ShaderProgram::setInt(const std::string& name, const glm::ivec4& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform4i(glGetUniformLocation(*p_programId, name.c_str()), value.x, value.y, value.z, value.w);
        }
    }

    void ShaderProgram::setUInt(const std::string& name, const glm::uvec1& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform1ui(glGetUniformLocation(*p_programId, name.c_str()), value.x);
        }
    }

    void ShaderProgram::setUInt(const std::string& name, const glm::uvec2& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform2ui(glGetUniformLocation(*p_programId, name.c_str()), value.x, value.y);
        }
    }

    void ShaderProgram::setUInt(const std::string& name, const glm::uvec3& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform3ui(glGetUniformLocation(*p_programId, name.c_str()), value.x, value.y, value.z);
        }
    }

    void ShaderProgram::setUInt(const std::string& name, const glm::uvec4& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform4ui(glGetUniformLocation(*p_programId, name.c_str()), value.x, value.y, value.z, value.w);
        }
    }

    void ShaderProgram::setFloat(const std::string& name, const glm::vec1& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform1f(glGetUniformLocation(*p_programId, name.c_str()), value.x);
        }
    }

    void ShaderProgram::setFloat(const std::string& name, const glm::vec2& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform2f(glGetUniformLocation(*p_programId, name.c_str()), value.x, value.y);
        }
    }

    void ShaderProgram::setFloat(const std::string& name, const glm::vec3& v) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform3f(glGetUniformLocation(*p_programId, name.c_str()), v.x, v.y, v.z);
        }
    }

    void ShaderProgram::setFloat(const std::string& name, const glm::vec4& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniform4f(glGetUniformLocation(*p_programId, name.c_str()), value.x, value.y, value.z, value.w);
        }
    }

    void ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const {
        if (p_programId != nullptr) {
            glUseProgram(*p_programId);
            glUniformMatrix4fv(glGetUniformLocation(*p_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
        }
    }

    bool operator==(const ShaderProgram& lhs, const ShaderProgram& rhs) noexcept {
        return lhs.p_programId == rhs.p_programId;
    }

    bool operator!=(const ShaderProgram& lhs, const ShaderProgram& rhs) noexcept {
        return !(lhs == rhs);
    }
} // namespace my