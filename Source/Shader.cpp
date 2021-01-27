#include <MyGL/Shader.hpp>
using namespace my;

Shader::Shader() : id(0), usable(false) {}

Shader::Shader(const std::string& path_to_vertex, const std::string& path_to_fragment) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(path_to_vertex);
        fShaderFile.open(path_to_fragment);
        std::stringstream vShaderStream, fShaderStream;
        // read file�s buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\nfilename : " << path_to_vertex << " " << path_to_fragment << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    usable = true;
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        usable = false;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        usable = false;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        usable = false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::Shader(bool from_string, const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    usable = true;

    const char* cVertexSource = vertexSource.c_str();
    const char* cFragmentSource = fragmentSource.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &cVertexSource, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        usable = false;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &cFragmentSource, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        usable = false;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        usable = false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

bool Shader::isUsable() const {
    return usable;
}

void Shader::use() const {
    glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUseProgram(id);
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUseProgram(id);
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUseProgram(id);
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, const glm::vec3& v) const {
    glUseProgram(id);
    glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}

void Shader::setFloat(const std::string& name, float v0, float v1, float v2) const {
    glUseProgram(id);
    glUniform3f(glGetUniformLocation(id, name.c_str()), v0, v1, v2);
}

void Shader::setFloat(const std::string& name, float v0, float v1, float v2, float v3) const {
    glUseProgram(id);
    glUniform4f(glGetUniformLocation(id, name.c_str()), v0, v1, v2, v3);
}

void Shader::setMat4(const std::string& name, const GLfloat* value) const {
    use();
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, value);
}
