#ifndef SKYBOX_H
#define SKYBOX_H

#include <iostream>
#include <string>

#include "Mesh.h"

class Renderer;



class Skybox : public Mesh
{
public:
	Skybox(const std::string& texturePath);
	~Skybox();

	void Render(const Renderer& renderer, const unsigned int& textureUnit) override;

protected:
	void ComputeVertices() override;
};



#endif // SKYBOX_H