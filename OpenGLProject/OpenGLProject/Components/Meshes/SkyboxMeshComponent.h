#ifndef SKYBOX_H
#define SKYBOX_H

#include <cstdint>

#include "MeshComponent.h"



class SkyboxMeshComponent : public MeshComponent
{
public:
	SkyboxMeshComponent();

private:
	// FacesCount is 6, verticesPerFaceCount is 6
	static constexpr int32_t VERTICES_COUNT = 36;

	void ComputeVertices();
};



#endif // SKYBOX_H