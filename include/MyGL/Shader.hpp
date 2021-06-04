#ifndef MY_SHADER
#define MY_SHADER

#include "mygl_export.h"

#include <string>
#include <unordered_map>

namespace my
{
    class MYGL_EXPORT Shader
    {
    private:
        static std::unordered_map<unsigned int, int> instancesCount;
        unsigned int m_shaderId = 0;
        bool m_usable = false;

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
        Shader() = default;

        /**
         * @brief Creates a shader with a given type and its source code in a c string
         * @param sourceCode A c-style string which holds the shader's source code
         * @param type The shader's type (one of Vertex, Fragment and Geometry)
         */
        Shader(const char* sourceCode, Type type);

        /**
         * @brief Copy constructor
         */
        Shader(const Shader& shader);

        /**
         * @brief Move constructor
         */
        Shader(Shader&& shader);

        /**
         * @brief Destructor
         *
         * The destructor deletes the underlying OpenGL shader object (it has no effect
         * on Shader programs which has already linked this shader)
         */
        ~Shader();

        /**
         * @brief Copy assignment operator
         */
        Shader& operator=(const Shader& shader);

        /**
         * @brief Move assignment operator
         */
        Shader& operator=(Shader&& shader);

        /**
         * @brief Loads a shader from a file
         * @param filename The name of the file containing the shader's source code
         * @param type The shader's type (one of Vertex, Fragment and Geometry)
         * @return True if the file was read succesfully and the shader was compiled
         *         without errors, false otherwise
         */
        bool loadFromFile(const std::string& filename, Type type);

        /**
         * @brief Loads a shader from a string
         * @param sourceCode A c-style string which holds the shader's source code
         * @param type The shader's type (one of Vertex, Fragment and Geometry
         * @return True if the shader was compiled without errors, false otherwise
         */
        bool loadFromString(const char* sourceCode, Type type);

        /**
         * @brief Indicates if the shader is usable
         * @return True if the shades is usable, false otherwise
         */
        bool isUsable() const;

        friend class ShaderProgram;
    };

} // namespace my

#endif // MY_SHADER
