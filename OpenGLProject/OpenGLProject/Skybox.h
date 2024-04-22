#ifndef SKYBOX_H
#define SKYBOX_H

#include <iostream>
#include <string>

#include "Mesh.h"

class Renderer;



class Skybox : public Mesh
{
public:
	Skybox();
	Skybox(const std::string& texturePath);
	~Skybox() override;

	void Render(const Renderer& renderer, const unsigned int textureUnit) const override;

protected:
	void ComputeVertices() override;

private:
	// FacesCount is 6, verticesPerFaceCount is 6
	static constexpr int VERTICES_COUNT = 36;
};



#endif // SKYBOX_H