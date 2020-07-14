#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>					// for cos and sin
#include <glm/gtc/constants.hpp>		// for pi

#include <vector>



class Sphere
{
private:
	// Number of double values that contains indexes vector
	int nbIndexes;
	// Vertex Buffer Object
	unsigned int VBO;
	// Vertex Array Object
	unsigned int VAO;
	// Element Buffer Object
	unsigned int EBO;
	// Radius of the planet [in kms]
	float radius;



public:
	Sphere(float radiusArg);

	void Draw();

	~Sphere();
};

#endif SPHERE_H
