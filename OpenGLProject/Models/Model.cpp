#include "Model.h"

#include <glm/mat4x4.hpp>

#include <cstddef> // std::size_t

#include "Buffers/VertexBuffer.h"
#include "ModelLoader.h"
#include "Rendering/Renderer.h"
#include "Scene/Transform.h"



Model::Model(const std::filesystem::path& inPath, const ShaderLookUpID::Enum inShaderLookUpID, const bool inGammaCorrection) :
	shaderLookUpID(inShaderLookUpID),
	gammaCorrection(inGammaCorrection)
{
	ModelLoader::LoadModel(*this, inPath);
}

void Model::StoreInstanceTransforms(const std::vector<Transform>& transforms) const
{
	std::vector<glm::mat4> modelMatrices;
	for (const Transform& transform : transforms)
	{
		modelMatrices.push_back(transform.Get());
	}

	// Configure instanced array
	VertexBuffer vbo(static_cast<const void*>(modelMatrices.data()), modelMatrices.size() * Transform::GetMatrixSizeInBytes());

	// Set transformation matrices as an instance vertex attribute for each mesh VAO already created
	for (const MeshComponent& mesh : meshes)
	{
		mesh.StoreInstanceTransforms();
	}

	vbo.Unbind();
}

void Model::Render() const
{
	for (const MeshComponent& mesh : meshes)
	{
		mesh.Render();
	}
}

void Model::RenderInstances(const uint32_t instanceCount) const
{
	for (const MeshComponent& mesh : meshes)
	{
		mesh.RenderInstances(instanceCount);
	}
}

void Model::AddMesh(MeshComponent&& mesh)
{
	meshes.emplace_back(mesh);
}

void Model::AddMaterial(BlinnPhongMaterial&& material)
{
	materials.emplace_back(material);
}
