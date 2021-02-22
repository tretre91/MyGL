#include <MyGL/Shader.hpp>
using namespace my;

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader, bool from_file) {
    std::string vShaderCode;
    std::string fShaderCode;

    if (from_file) {
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            std::stringstream vShaderStream, fShaderStream;

            // retrieve the vertex shader file's content
            vShaderFile.open(vertexShader);
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();

            // retrieve the vertex shader file's content
            fShaderFile.open(fragmentShader);
            fShaderStream << fShaderFile.rdbuf();
            fShaderFile.close();

            vShaderCode = vShaderStream.str();
            fShaderCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\nfilename : " << vertexShader << " " << fragmentShader << std::endl;
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
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        m_isUsable = false;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        m_isUsable = false;
    }

    m_shaderId = glCreateProgram();
    glAttachShader(m_shaderId, vertex);
    glAttachShader(m_shaderId, fragment);
    glLinkProgram(m_shaderId);

    glGetProgramiv(m_shaderId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        m_isUsable = false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

bool Shader::isUsable() const {
    return m_isUsable;
}

void Shader::use() const {
    glUseProgram(m_shaderId);
}

void Shader::setBool(const std::string& name, bool value) const {
    use();
    glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    use();
    glUniform1i(glGetUniformLocation(m_shaderId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    use();
    glUniform1f(glGetUniformLocation(m_shaderId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, const glm::vec3& v) const {
    use();
    glUniform3f(glGetUniformLocation(m_shaderId, name.c_str()), v.x, v.y, v.z);
}

void Shader::setFloat(const std::string& name, float v0, float v1, float v2) const {
    use();
    glUniform3f(glGetUniformLocation(m_shaderId, name.c_str()), v0, v1, v2);
}

void Shader::setFloat(const std::string& name, float v0, float v1, float v2, float v3) const {
    use();
    glUniform4f(glGetUniformLocation(m_shaderId, name.c_str()), v0, v1, v2, v3);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    use();
    glUniformMatrix4fv(glGetUniformLocation(m_shaderId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
