#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>

#include <glm/glm.hpp>					// for cos and sin
#include <vector>

#include "VertexBuffer.h"
#include "VertexArray.h"



class Skybox
{
private:
	std::vector<float> vertCoor;
	VertexArray * vao;

	void Compute();
	void Store();

public:
	Skybox();
	~Skybox();

	void Draw();
};

#endif SKYBOX_H