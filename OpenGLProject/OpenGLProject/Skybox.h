#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>

#include <glm/glm.hpp>					// for cos and sin
#include <vector>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"



class Skybox
{
private:
	std::vector<float> vertCoor;
	VertexArray * vao;
	Texture * texture;

	void Compute();
	void Store();

public:
	Skybox(const char * path);
	~Skybox();

	void Render();

	inline Texture GetTexture() const { return *texture; }
};

#endif SKYBOX_H