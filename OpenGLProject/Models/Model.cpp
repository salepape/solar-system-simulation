#include "Model.h"

#include <glm/mat4x4.hpp>

#include <cassert>
#include <cstddef> // std::size_t
#include <iostream>
#include <utility> // std::forward

#include "Buffers/VertexBuffer.h"
#include "ModelLoader.h"
#include "Rendering/Renderer.h"
#include "Scene/Transform.h"



Model::Model(const std::filesystem::path& inPath, const ShaderLookUpID::Enum inShaderLookUpID, const bool inGammaCorrection) :
	shaderLookUpID(inShaderLookUpID),
	gammaCorrection(inGammaCorrection)
{
	ModelLoader::LoadModel(*this, inPath);

	if (meshes.size() <= 0)
	{
		std::cout << "ERROR::MODEL - No meshes have been stored in Model after loading!" << std::endl;
		assert(false);
	}
	else if (materials.size() <= 0)
	{
		std::cout << "ERROR::MODEL - No materials have been stored in Model after loading!" << std::endl;
		assert(false);
	}
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
	// Make use of a forward reference so the value type (i.e. rvalue here) is preserved,
	// calling the move cstr instead of the copy one
	meshes.emplace_back(std::forward<MeshComponent>(mesh));
}

void Model::AddMaterial(BlinnPhongMaterial&& material)
{
	materials.emplace_back(std::forward<BlinnPhongMaterial>(material));
}
