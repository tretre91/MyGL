#ifndef MY_SHADER
#define MY_SHADER

#include "Header.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace my 
{

    class MYGL_EXPORT Shader {
    private:
        unsigned int id = 0;
        bool usable = false;

    public:

        /**
         * @brief Creates an emty shader program, it is not usable as is
        */
        Shader() = default;

        /**
         * @brief Creates a shader program from a vertex and a fragment shader
         * @details The shader's source code can be stored in a file or in a string,
         *          the storage method should be the same for both the vertex shader's
         *          code and the fragment shader's
         * 
         * @param vertexShader The path to the file containing the vertex shader source code
         * @param fragmentShader The path to the file containing the fragment shader source code
         * @param from_file Indicates if vertexShader and fragmentShaders are filenames,
         *                  true means that the code is stored in files, false means that
         *                  the variables hold the source code
        */
        Shader(const std::string& vertexShader, const std::string& fragmentShader, bool from_file = true);

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
