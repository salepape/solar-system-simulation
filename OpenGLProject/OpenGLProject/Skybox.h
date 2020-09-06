#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>

#include <glm/glm.hpp>					// for cos and sin
#include <vector>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Renderer.h"


class Skybox
{
private:
	VertexArray * vao;
	Texture * texture;

	std::vector<float> vertCoor;
	Texture * texture;

	void Compute();
	void Store();

public:
	Skybox(const char * path);
	~Skybox();

	void Render(Renderer& renderer, unsigned int& textureUnit);
};

#endif SKYBOX_H