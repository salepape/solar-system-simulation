#ifndef MODEL_H
#define MODEL_H

#include <glm/mat4x4.hpp>

#include <cstddef> // std::size_t
#include <cstdint>
#include <filesystem>
#include <vector>

#include "Components/Meshes/MeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "Rendering/ShaderLoader.h"
#include "Rendering/Texture.h"



// Set of Meshes with Materials already applied from a 3D Software (e.g. Blender, Maya, etc.)
class Model
{
public:
	Model(const std::filesystem::path& inPath, const ShaderLookUpID::Enum inShaderLookUpID, const bool inGammaCorrection = false);

	void StoreInstanceModelMatrices(const std::vector<glm::mat4>& modelMatrices, const std::size_t sizeInBytes) const;

	void Render() const;
	void RenderInstances(const uint32_t instanceCount) const;

	const std::vector<BlinnPhongMaterial>& GetMaterials() const { return materials; }
	ShaderLookUpID::Enum GetShaderLookUpID() const { return shaderLookUpID; }

	void AddMesh(MeshComponent&& mesh);
	void AddMaterial(BlinnPhongMaterial&& material);

private:
	// List of Sub-Meshes that represents the Model (retrieved from .obj file)
	// Warning: model is supposed to be simple enough, i.e. only contains 1 Mesh, 1 Material definition, 1 Texture
	std::vector<MeshComponent> meshes;

	// List of Materials that applies to each Sub-Mesh of the Model (retrieved from .mtl file), as per ASSIMP convention
	// Warning: model is supposed to be simple enough, i.e. only contains 1 Mesh, 1 Material definition, 1 Texture
	std::vector<BlinnPhongMaterial> materials;
	ShaderLookUpID::Enum shaderLookUpID;

	[[maybe_unused]] bool gammaCorrection{ false };
};



#endif // MODEL_H
