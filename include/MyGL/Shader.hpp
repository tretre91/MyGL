#ifndef MY_SHADER
#define MY_SHADER

#include "mygl_export.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace my
{
    class MYGL_EXPORT Shader
    {
    private:
        unsigned int m_shaderId = 0;
        bool m_isUsable = false;

    public:
        /**
         * @brief Creates an emty shader program, it is not usable as is
         */
        Shader() = default;

        /**
         * @brief Creates a shader program from a vertex and a fragment shader
         * @details The shader's source code can be stored in a file or in a string,
         *          the storage method should be the same for both the vertex shader
         *          and fragment shader's code
         *
         * @param vertexShader The path to the file containing the vertex shader source code
         * @param fragmentShader The path to the file containing the fragment shader source code
         * @param fromFile Indicates if vertexShader and fragmentShaders are filenames,
         *                 true means that the code is stored in files, false means that
         *                 the variables hold the source code
         */
        Shader(const std::string& vertexShader, const std::string& fragmentShader, bool fromFile = true);

        /**
         * @brief Tells wether the shader program has been correctly initialized
         * @return true if the program can be used
         */
        bool isUsable() const;

        /**
         * @brief Sets this shader to be the active shader program
         */
        void use() const;

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
         * @param v0,values The new values to assign
         *
         * @{
         */

        /** @brief Sets the value of a 4x4 matrix uniform */
        void setMat4(const std::string& name, const glm::mat4& value) const;

        /** @name Int uniform functions */
        /** @{ */

        /**
         * @brief Sets the value of an integer uniform
         */
        void setInt(const std::string& name, const glm::ivec1& value) const;
        /**
         * @brief Sets the value of a 2 component integer vector (ivec2) uniform
         */
        void setInt(const std::string& name, const glm::ivec2& value) const;
        /**
         * @brief Sets the value of a 3 component integer vector (ivec3) uniform
         */
        void setInt(const std::string& name, const glm::ivec3& value) const;
        /**
         * @brief Sets the value of a 4 component integer vector (ivec4) uniform
         */
        void setInt(const std::string& name, const glm::ivec4& value) const;
        /**
         * @brief Sets the value of an integer uniform using multiple values
         * @param v0 The first value
         * @param ...values The remaining values (up to 3)
         */
        template<typename... Args>
        inline void setInt(const std::string& name, int v0, Args... values) const {
            setInt(name, glm::vec<1 + sizeof...(Args), int>(v0, values...));
        }

        /** @} */

        /** @name Unsigned int uniform functions */
        /** @{ */

        /**
         * @brief Sets the value of an unsigned integer uniform
         */
        void setUInt(const std::string& name, const glm::uvec1& value) const;
        /**
         * @brief Sets the value of a 2 component unsigned integer vector (uvec2) uniform
         */
        void setUInt(const std::string& name, const glm::uvec2& value) const;
        /**
         * @brief Sets the value of a 3 component unsigned integer vector (uvec3) uniform
         */
        void setUInt(const std::string& name, const glm::uvec3& value) const;
        /**
         * @brief Sets the value of a 4 component unsigned integer vector (uvec4) uniform
         */
        void setUInt(const std::string& name, const glm::uvec4& value) const;
        /**
         * @brief Sets the value of an unsigned integer uniform using multiple values
         * @param v0 The first value
         * @param ...values The remaining values (up to 3)
         */
        template<typename... Args>
        inline void setUInt(const std::string& name, unsigned int v0, Args... values) const {
            setUInt(name, glm::vec<1 + sizeof...(values), unsigned int>(v0, values...));
        }

        /** @} */

        /** @name Float uniform functions */
        /** @{ */

        /**
         * @brief Sets the value of a float uniform
         */
        void setFloat(const std::string& name, const glm::vec1& value) const;
        /**
         * @brief Sets the value of a 2 component float vector (vec2) uniform
         */
        void setFloat(const std::string& name, const glm::vec2& value) const;
        /**
         * @brief Sets the value of a 3 component float vector (vec3) uniform
         */
        void setFloat(const std::string& name, const glm::vec3& value) const;
        /**
         * @brief Sets the value of a 4 component float vector (vec4) uniform
         */
        void setFloat(const std::string& name, const glm::vec4& value) const;
        /**
         * @brief Sets the value of a float uniform using multiple values
         * @param v0 The first value
         * @param ...values The remaining values (up to 3)
         */
        template<typename... Args>
        inline void setFloat(const std::string& name, float v0, Args... values) const {
            setFloat(name, glm::vec<1 + sizeof...(Args), float>(v0, values...));
        }

        /** @} */

        /** @} */
    };

} // namespace my

#endif // MY_SHADER
