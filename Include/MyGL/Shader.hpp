#ifndef MY_SHADER
#define MY_SHADER

#include "Header.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace my 
{

    class Shader {
    private:
        unsigned int id;
        bool usable;

    public:

        Shader();

        /**
         * @brief Creates a shader program from a vertex and a fragment shader stored in files
         * @param path_to_vertex The path to the file containing the vertex shader source code
         * @param path_to_fragment The path to the file containing the fragment shader source code
        */
        Shader(const std::string& path_to_vertex, const std::string& path_to_fragment);

        /**
         * @brief Creates a shader program from 2 strings containing the source code
         * @param vertexSource The string containing the vertex shader source code
         * @param fragmentSource The string containing the fragment shader source code
        */
        Shader(bool from_string, const std::string& vertexSource, const std::string& fragmentSource);

        /**
         * @brief Tells wether the shader program has been correctly initialized
         * @return true if the program can be used
        */
        bool isUsable() const;

        /**
         * @brief Sets this shader to be the active shader program
        */
        void use() const;

        /**
         * @brief Sets the bool uniform named 'name' to 'value'
         * @param name The name of the uniform we want to modify 
         * @param value The new value of the uniform
        */
        void setBool(const std::string& name, bool value) const;

        /**
         * @brief Sets the int uniform named 'name' to 'value'
         * @param name The name of the uniform we want to modify 
         * @param value The new value of the uniform
        */
        void setInt(const std::string& name, int value) const;

        /**
         * @brief Sets the float uniform named 'name' to 'value'
         * @param name The name of the uniform we want to modify
         * @param value The new value of the uniform
        */
        void setFloat(const std::string& name, float value) const;

        void setFloat(const std::string& name, const glm::vec3& value) const;
        void setFloat(const std::string& name, float v0, float v1, float v2) const;

        void setFloat(const std::string& name, float v0, float v1, float v2, float v3) const;

        /**
         * @brief Sets a 4 by 4 matrix uniform
         * @param name The name of the uniform we want to modify
         * @param value The new value of the uniform
        */
        void setMat4(const std::string& name, const GLfloat* value) const;
    };

}

#endif //MY_SHADER
