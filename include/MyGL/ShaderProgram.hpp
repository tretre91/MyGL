#ifndef MYGL_SHADER_PROGRAM
#define MYGL_SHADER_PROGRAM

#include "mygl_export.h"
#include "Shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <memory>
#include <string>

namespace my
{
    /**
     * @brief Class for creating shader programs
     */
    class ShaderProgram
    {
    private:
        struct ShaderProgramDeleter {
            void operator()(unsigned int* programId) {
                if (programId != nullptr) {
                    glDeleteProgram(*programId);
                    delete programId;
                }
            }
        };

        std::shared_ptr<unsigned int> p_programId;

    public:
        /**
         * @brief Creates an empty shader program, it is not usable as is
         */
        MYGL_EXPORT ShaderProgram() noexcept = default;

        /**
         * @brief Adds one ore more shaders to this program
         * @param shader,shaders The shaders to add to this program
         */
        template<typename... Shaders>
        inline void addShaders(Shader shader, Shaders... shaders);

        /**
         * @brief Links all the added shaders to create a usable shader program
         * @return True if the linking was successful, false otherwise
         */
        MYGL_EXPORT bool link();

        /**
         * @brief Tells wether the shader program is usable
         * @return true if the program can be used
         */
        MYGL_EXPORT bool isUsable() const noexcept;

        /**
         * @brief Sets this program to be the active shader program
         */
        MYGL_EXPORT void use() const;

        /** @name Uniform related functions
         * These functions are used to modify a shader's uniform value, they take
         * the uniform's name and the new value as parameters.
         *
         * @note You can use these functions to set boolean uniforms, as integral types
         *       are implicitely converted to bool in glsl (0 or 0.0 is converted to false,
         *       anything else is converted to true)
         *
         * @param name The uniform's name
         * @param value The new value to assign to the uniform
         * @param v0,value(s) The new values to assign
         *
         * @{
         */

        /** @brief Sets the value of a 4x4 matrix uniform */
        MYGL_EXPORT void setMat4(const std::string& name, const glm::mat4& value) const;

        /** @name Int uniform functions */
        /** @{ */

        /**
         * @brief Sets the value of an integer uniform
         */
        MYGL_EXPORT void setInt(const std::string& name, const glm::ivec1& value) const;
        /**
         * @brief Sets the value of a 2 component integer vector (ivec2) uniform
         */
        MYGL_EXPORT void setInt(const std::string& name, const glm::ivec2& value) const;
        /**
         * @brief Sets the value of a 3 component integer vector (ivec3) uniform
         */
        MYGL_EXPORT void setInt(const std::string& name, const glm::ivec3& value) const;
        /**
         * @brief Sets the value of a 4 component integer vector (ivec4) uniform
         */
        MYGL_EXPORT void setInt(const std::string& name, const glm::ivec4& value) const;
        /**
         * @brief Sets the value of an integer uniform using multiple values
         * @param name The uniform's name
         * @param v0 The first value
         * @param values The remaining values (up to 3)
         */
        template<typename... Args>
        inline void setInt(const std::string& name, int v0, Args... values) const;

        /** @} */

        /** @name Unsigned int uniform functions */
        /** @{ */

        /**
         * @brief Sets the value of an unsigned integer uniform
         */
        MYGL_EXPORT void setUInt(const std::string& name, const glm::uvec1& value) const;
        /**
         * @brief Sets the value of a 2 component unsigned integer vector (uvec2) uniform
         */
        MYGL_EXPORT void setUInt(const std::string& name, const glm::uvec2& value) const;
        /**
         * @brief Sets the value of a 3 component unsigned integer vector (uvec3) uniform
         */
        MYGL_EXPORT void setUInt(const std::string& name, const glm::uvec3& value) const;
        /**
         * @brief Sets the value of a 4 component unsigned integer vector (uvec4) uniform
         */
        MYGL_EXPORT void setUInt(const std::string& name, const glm::uvec4& value) const;
        /**
         * @brief Sets the value of an unsigned integer uniform using multiple values
         * @param name The uniform's name
         * @param v0 The first value
         * @param values The remaining values (up to 3)
         */
        template<typename... Args>
        inline void setUInt(const std::string& name, unsigned int v0, Args... values) const;

        /** @} */

        /** @name Float uniform functions */
        /** @{ */

        /**
         * @brief Sets the value of a float uniform
         */
        MYGL_EXPORT void setFloat(const std::string& name, const glm::vec1& value) const;
        /**
         * @brief Sets the value of a 2 component float vector (vec2) uniform
         */
        MYGL_EXPORT void setFloat(const std::string& name, const glm::vec2& value) const;
        /**
         * @brief Sets the value of a 3 component float vector (vec3) uniform
         */
        MYGL_EXPORT void setFloat(const std::string& name, const glm::vec3& value) const;
        /**
         * @brief Sets the value of a 4 component float vector (vec4) uniform
         */
        MYGL_EXPORT void setFloat(const std::string& name, const glm::vec4& value) const;
        /**
         * @brief Sets the value of a float uniform using multiple values
         * @param name The uniform's name
         * @param v0 The first value
         * @param values The remaining values (up to 3)
         */
        template<typename... Args>
        inline void setFloat(const std::string& name, float v0, Args... values) const;

        /** @} */

        /** @} */

        MYGL_EXPORT friend bool operator==(const ShaderProgram& lhs, const ShaderProgram& rhs) noexcept;
        MYGL_EXPORT friend bool operator!=(const ShaderProgram& lhs, const ShaderProgram& rhs) noexcept;
    };

    template<typename... Shaders>
    inline void ShaderProgram::addShaders(Shader shader, Shaders... shaders) {
        if (p_programId == nullptr) {
            const unsigned int programId = glCreateProgram();
            if (programId == 0) {
                std::cerr << "ERROR::SHADER PROGRAM: failed to create a shader program\n";
                return;
            }
            p_programId.reset(new unsigned int{programId}, ShaderProgramDeleter());
        }

        for (const Shader& s : {shader, shaders...}) {
            if (!s.isUsable()) {
                std::cerr << "ERROR::SHADER PROGRAM: one of the provided shaders is not usable\n";
                return;
            }
            glAttachShader(*p_programId, *s.p_shaderId);
        }
    }

    template<typename... Args>
    inline void ShaderProgram::setInt(const std::string& name, int v0, Args... values) const {
        setInt(name, glm::vec<1 + sizeof...(Args), int>(v0, values...));
    }

    template<typename... Args>
    inline void ShaderProgram::setUInt(const std::string& name, unsigned int v0, Args... values) const {
        setUInt(name, glm::vec<1 + sizeof...(values), unsigned int>(v0, values...));
    }

    template<typename... Args>
    inline void ShaderProgram::setFloat(const std::string& name, float v0, Args... values) const {
        setFloat(name, glm::vec<1 + sizeof...(Args), float>(v0, values...));
    }

} // namespace my

#endif // MYGL_SHADER_PROGRAM
