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

	void Render(const Renderer& renderer, const uint32_t textureUnit) const override;

protected:
	void ComputeVertices() override;

private:
	// FacesCount is 6, verticesPerFaceCount is 6
	static constexpr int VERTICES_COUNT = 36;
};



#endif // SKYBOX_H