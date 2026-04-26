#include "Model.h"

#include "Buffers/VertexBuffer.h"
#include "ModelLoader.h"
#include "Rendering/Renderer.h"



Model::Model(const std::filesystem::path& inPath, const ShaderLookUpID::Enum inShaderLookUpID, const bool inGammaCorrection) :
	shaderLookUpID(inShaderLookUpID), gammaCorrection(inGammaCorrection)
{
	ModelLoader::LoadModel(*this, inPath);
}

void Model::StoreInstanceModelMatrices(const std::vector<glm::mat4>& modelMatrices, const std::size_t sizeInBytes) const
{
	// Configure instanced array
	VertexBuffer vbo(static_cast<const void*>(modelMatrices.data()), sizeInBytes);

	// Set transformation matrices as an instance vertex attribute for each mesh VAO already created
	for (const MeshComponent& mesh : meshes)
	{
		mesh.StoreInstanceModelMatrices();
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
