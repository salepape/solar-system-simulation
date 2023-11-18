#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"



class Skybox
{
private:
	std::vector<float> vertCoor;
	Texture* texture;

	VertexArray* vao;

	void Compute();
	void Store();

public:
	Skybox(const char* path);
	~Skybox();

	void Render(const Renderer& renderer, const unsigned int& textureUnit);
};



#endif // SKYBOX_H