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
	// Radius of the planet 
	float radius;

	// Vectors containing sphere data computed
	std::vector<float> vertCoor;
	std::vector<float> normalCoor;
	std::vector<float> textCoor;
	std::vector<int> indexes;


public:
	Sphere(float radiusArg);

	void Compute();
	void Linking();
	void Draw();

	~Sphere();
};

#endif SPHERE_H
