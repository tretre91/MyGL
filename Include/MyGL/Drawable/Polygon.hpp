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

			for (unsigned int i = 3; i < 3 * (sides + 1); i += 3) {
				vertices[i] = glm::cos(angle);
				vertices[i + 1] = glm::sin(angle);
				vertices[i + 2] = 0.0f;
				angle += inc;
			}

			for (unsigned int i = 0; i < sides + 1; i++) indices[i] = i;
			indices[sides + 1] = 1;
		}

		static unsigned int VAO, VBO, EBO;
		static std::array<float, 3 * (sides + 1)> vertices;
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

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
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
		Polygon(int radius, int x, int y) : AbstractShape(radius, radius, x, y) {
			if (!initialized) {
				computeVertices();
				glInit();
				initialized = true;
			}
		}

		/**
		 * @brief Gives a list of the polygon's points
		 * @return A std::vector containing every point (represented by a glm::vec2) in counter-clockwise order
		*/
		virtual std::vector<glm::vec2> points() const {
			glm::mat4 transform;
			if (updateMatrix) {
				transform = glm::mat4(1.0f);
				transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
				transform = glm::rotate(transform, glm::radians((float)rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
				transform = glm::scale(transform, glm::vec3(originalScale.x * scaleFactor.x, originalScale.y * scaleFactor.y, 1.0f));
			}
			else {
				transform = model;
			}

			std::vector<glm::vec2> res;
			for (size_t i = 3; i < vertices.size(); i += 3) {
				glm::vec4 vertex = transform * glm::vec4(vertices[i], vertices[i + 1], 0.0f, 1.0f);
				res.push_back(glm::vec2(vertex.x, vertex.y));
			}

			return res;
		}

		/**
		 * @brief Draws a polygon, this method is called by a window
		 * @param lookAt The view matrix (usually provided by the window)
		 * @param projection The projection matrix (also provided by the window)
		*/
		virtual void draw(const glm::mat4& lookAt, const glm::mat4& projection) {
			if (updateMatrix) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
				model = glm::rotate(model, glm::radians((float)rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, glm::vec3(originalScale.x * scaleFactor.x, originalScale.y * scaleFactor.y, 1.0f));
				updateMatrix = false;
			}

			shader.setMat4("model", glm::value_ptr(model));
			shader.setMat4("view", glm::value_ptr(lookAt));
			shader.setMat4("projection", glm::value_ptr(projection));
			shader.setFloat("color", color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.alpha / 255.0f);
			shader.use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLE_FAN, sides + 2, GL_UNSIGNED_INT, 0);
		}
	};

	template<unsigned int sides>
	unsigned int Polygon<sides>::VAO = 0;

	template<unsigned int sides>
	unsigned int Polygon<sides>::VBO = 0;

	template<unsigned int sides>
	unsigned int Polygon<sides>::EBO = 0;

	template<unsigned int sides>
	std::array<float, 3 * (sides + 1)> Polygon<sides>::vertices = std::array<float, 3 * (sides + 1)>();

	template<unsigned int sides>
	std::array<unsigned int, sides + 2> Polygon<sides>::indices = std::array<unsigned int, sides + 2>();

	template<unsigned int sides>
	bool Polygon<sides>::initialized = false;
} //namespace my

#endif //MY_POLYGON