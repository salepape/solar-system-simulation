#ifndef SKYBOX_H
#define SKYBOX_H

#include <cstdint>

#include "MeshComponent.h"

class Renderer;



class SkyboxMeshComponent : public MeshComponent
{
public:
	SkyboxMeshComponent();

	void Render(const Renderer& renderer) const override;

private:
	// FacesCount is 6, verticesPerFaceCount is 6
	static constexpr int32_t VERTICES_COUNT = 36;

	void ComputeVertices();
};



#endif // SKYBOX_H