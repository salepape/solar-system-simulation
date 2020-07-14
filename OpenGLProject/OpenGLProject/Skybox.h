#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>

#include <glm/glm.hpp>					// for cos and sin



class Skybox
{
private:
	// Vertex Buffer Object
	unsigned int VBO;
	// Vertex Array Object
	unsigned int VAO;

public:
	Skybox();

	void Draw();

	~Skybox();
};

#endif SKYBOX_H