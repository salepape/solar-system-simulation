#ifndef SKYBOX_H
#define SKYBOX_H

#include "Mesh.h"

class Renderer;



class Skybox : public Mesh
{
public:
	Skybox();

	void Render(const Renderer& renderer) const override;

protected:
	void ComputeVertices() override;

private:
	// FacesCount is 6, verticesPerFaceCount is 6
	static constexpr int32_t VERTICES_COUNT = 36;
};



#endif // SKYBOX_H