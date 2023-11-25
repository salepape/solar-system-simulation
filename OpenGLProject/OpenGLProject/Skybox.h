#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"



class Skybox : public Mesh
{
private:
	Texture* texture{ nullptr };

protected:
	void Compute() override;

public:
	Skybox(const char* path);
	~Skybox();

	void Render(const Renderer& renderer, const unsigned int& textureUnit) override;
};



#endif // SKYBOX_H