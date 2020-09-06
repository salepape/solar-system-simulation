#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>					// for cos and sin
#include <glm/gtc/constants.hpp>		// for pi

#include <vector>

#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


class Sphere
{
private:
	// Number of double values that contains indexes vector
	int nbIndexes;
	// Radius of the planet 
	float radius;

	VertexArray * vao;
	IndexBuffer * ibo;

	// Vectors containing sphere data computed
	std::vector<float> vertCoor;
	std::vector<float> normalCoor;
	std::vector<float> textCoor;
	std::vector<int> indexes;
	Texture * texture;

	void Compute();
	void Store();

public:
	Sphere(const char * texturePath, float radiusArg);
	~Sphere();

	void Draw();

	inline Texture GetTexture() const { return *texture; }
};

#endif SPHERE_H
