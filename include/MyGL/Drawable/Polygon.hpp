#ifndef MY_POLYGON
#define MY_POLYGON

#include "../Shader.hpp"
#include "AbstractShape.hpp"

namespace my
{
    /**
     * @brief Class for creating regular polygons of 3 or more sides (the number of sides must be specified in the template argument)
    */
    template<unsigned int sides>
    class Polygon : public AbstractShape {
    private:
        static void computeVertices() {
            float angle = 0.0f;
            float inc = (2 * pi) / sides;

            //the center's coordinates
            vertices[0] = 0.0f;
            vertices[1] = 0.0f;
            vertices[2] = 0.0f;
            vertices[3] = 0.5f;
            vertices[4] = 0.5f;

            for (size_t i = 5; i < 5 * (sides + 1); i += 5) {
                vertices[i] = glm::cos(angle);
                vertices[i + 1] = glm::sin(angle);
                vertices[i + 2] = 0.0f;
                vertices[i + 3] = (vertices[i] + 1.0f) / 2.0f;
                vertices[i + 4] = (vertices[i + 1] + 1.0f) / 2.0f;
                angle += inc;
            }

            for (size_t i = 0; i < sides + 1; i++) indices[i] = static_cast<unsigned int>(i);
            indices[sides + 1] = 1;
        }

        static unsigned int VAO;
        static unsigned int VBO;
        static unsigned int EBO;
        static std::array<float, 5 * (sides + 1)> vertices;
        static std::array<unsigned int, sides + 2> indices;

        static bool initialized;

        void glInit() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }

        /**
         * @brief Gives a list of the polygon's points
         * @return A std::vector containing every point (represented by a glm::vec2) in counter-clockwise order
        */
        virtual std::vector<glm::vec2> points() const {
            glm::mat4 transform;
            if (m_updateMatrix) {
                transform = glm::mat4(1.0f);
                transform = glm::translate(transform, glm::vec3(m_position.x, m_position.y, 0.0f));
                transform = glm::rotate(transform, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
                transform = glm::scale(transform, glm::vec3(m_originalScale.x * m_scaleFactor.x, m_originalScale.y * m_scaleFactor.y, 1.0f));
            }
            else {
                transform = m_model;
            }

            std::vector<glm::vec2> res;
            res.reserve(sides);
            for (size_t i = 5; i < vertices.size(); i += 5) {
                glm::vec4 vertex = transform * glm::vec4(vertices[i], vertices[i + 1], 0.0f, 1.0f);
                res.emplace_back(glm::vec2(vertex.x, vertex.y));
            }

            return res;
        }

    public:

        /**
         * @brief Creates a 'n' sided polygon of radius 10
        */
        Polygon() : AbstractShape() {
            if (!initialized) {
                computeVertices();
                glInit();
                initialized = true;
            }
        }

        /**
         * @brief Creates a regular polygon with the specified radius
         * @param radius The radius of this polygon
        */
        Polygon(int radius) : AbstractShape(radius, radius) {
            if (!initialized) {
                computeVertices();
                glInit();
                initialized = true;
            }
        }

        /**
         * @brief Creates a regular polygon with the specified radius and places its center at (x, y)
         * @param radius The polygon's radius
         * @param x The x coordinate of the center
         * @param y The y coordinate of the center
        */
        Polygon(int radius, int x, int y) : AbstractShape(radius, radius) {
            if (!initialized) {
                computeVertices();
                glInit();
                initialized = true;
            }
            setPosition(x, y, true);
        }

        /**
         * @brief Draws a polygon, this method is called by a window
         * @param lookAt The view matrix (usually provided by the window)
         * @param projection The projection matrix (also provided by the window)
        */
        virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection) {
            if (m_updateMatrix) {
                m_model = glm::mat4(1.0f);
                m_model = glm::translate(m_model, glm::vec3(m_position.x, m_position.y, 0.0f));
                m_model = glm::rotate(m_model, glm::radians(m_rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
                if (m_outlineThickness > 0) {
                    m_outlineModel = glm::scale(m_model, glm::vec3(static_cast<float>(m_outlineThickness) + m_originalScale.x * m_scaleFactor.x, static_cast<float>(m_outlineThickness) + m_originalScale.y * m_scaleFactor.y, 1.0f));
                }
                m_model = glm::scale(m_model, glm::vec3(m_originalScale.x * m_scaleFactor.x, m_originalScale.y * m_scaleFactor.y, 1.0f));
                m_updateMatrix = false;
            }
            glActiveTexture(GL_TEXTURE3);
            m_texture.bind();
            p_activeShader->setMat4("model", m_model);
            p_activeShader->setMat4("view", lookAt);
            p_activeShader->setMat4("projection", projection);
            p_activeShader->setFloat("color", m_color.getNormalized());
            p_activeShader->use();

            if (m_outlineThickness > 0) {
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLE_FAN, sides + 2, GL_UNSIGNED_INT, 0);

                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                shader.setMat4("model", m_outlineModel);
                shader.setFloat("color", m_outlineColor.getNormalized());
                if (m_isTextured) {
                    shader.setMat4("view", lookAt);
                    shader.setMat4("projection", projection);
                }
                shader.use();

                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLE_FAN, sides + 2, GL_UNSIGNED_INT, 0);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);
                glClear(GL_STENCIL_BUFFER_BIT);
            } else {
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLE_FAN, sides + 2, GL_UNSIGNED_INT, 0);
            }
        }
    };

    template<unsigned int sides>
    unsigned int Polygon<sides>::VAO = 0;

    template<unsigned int sides>
    unsigned int Polygon<sides>::VBO = 0;

    template<unsigned int sides>
    unsigned int Polygon<sides>::EBO = 0;

    template<unsigned int sides>
    std::array<float, 5 * (sides + 1)> Polygon<sides>::vertices = std::array<float, 5 * (sides + 1)>();

    template<unsigned int sides>
    std::array<unsigned int, sides + 2> Polygon<sides>::indices = std::array<unsigned int, sides + 2>();

    template<unsigned int sides>
    bool Polygon<sides>::initialized = false;
} //namespace my

#endif //MY_POLYGON