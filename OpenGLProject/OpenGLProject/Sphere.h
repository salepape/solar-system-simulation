#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"



constexpr int nbParalStrips = 100;
constexpr int nbMeridStrips = 100;

class Sphere
{
private:
	int nbIndexes{ 0 };
	float radius{ 0.0f };

	VertexArray* vao{ nullptr };
	IndexBuffer* ibo{ nullptr };

	std::vector<float> vertCoor;
	std::vector<float> normalCoor;
	std::vector<float> textCoor;
	std::vector<int> indexes;
	Texture* texture{ nullptr };

	void Compute();
	void Store();

public:
	Sphere(const char* path, const float radiusArg);
	~Sphere();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);
};



#endif // SPHERE_H
