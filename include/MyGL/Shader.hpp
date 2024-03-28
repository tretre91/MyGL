#ifndef MYGL_SHADER
#define MYGL_SHADER

#include "mygl_export.h"

#include <memory>
#include <string>

namespace my
{
    /**
     * @brief Class for storing a shader
     */
    class Shader
    {
    private:
        std::shared_ptr<unsigned int> p_shaderId;

    public:
        enum class Type
        {
            Vertex,
            Fragment,
            Geometry
        };

        /**
         * @brief Creates an empty shader
         */
        MYGL_EXPORT Shader() noexcept = default;

        /**
         * @brief Creates a shader with a given type and its source code in a c string
         * @param sourceCode A string which holds the shader's source code
         * @param type The shader's type (one of Vertex, Fragment and Geometry)
         */
        MYGL_EXPORT Shader(const std::string& sourceCode, Type type);

        /**
         * @brief Destructor
         *
         * The destructor deletes the underlying OpenGL shader object (it has no effect
         * on Shader programs which have already linked this shader).
         */
        MYGL_EXPORT ~Shader() = default;

        /**
         * @brief Loads a shader from a file
         * @param filename The name of the file containing the shader's source code
         * @param type The shader's type (one of Vertex, Fragment and Geometry)
         * @return True if the file was read succesfully and the shader was compiled
         *         without errors, false otherwise
         */
        MYGL_EXPORT bool loadFromFile(const std::string& filename, Type type);

        /**
         * @brief Loads a shader from a string
         * @param sourceCode A string which holds the shader's source code
         * @param type The shader's type (one of Vertex, Fragment and Geometry
         * @return True if the shader was compiled without errors, false otherwise
         */
        MYGL_EXPORT bool loadFromString(const std::string& sourceCode, Type type);

        /**
         * @brief Indicates if the shader is usable
         * @return True if the shades is usable, false otherwise
         */
        MYGL_EXPORT bool isUsable() const noexcept;

        friend class ShaderProgram;
    };

} // namespace my

#endif // MYGL_SHADER
