#include "MeshComponent.h"

#include <glad/glad.h>

#include <cassert>
#include <iostream>
#include <utility>

#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "Rendering/Renderer.h"



MeshComponent::MeshComponent(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices) :
	vertices(inVertices), indices(inIndices)
{
	StoreVertices();
}

void MeshComponent::StoreVertices()
{
	if (vertices.empty())
	{
		std::cout << "ERROR::MESH - No vertices found!" << std::endl;
		assert(false);
	}

	vao = std::make_shared<VertexArray>();
	VertexBuffer vbo(static_cast<const void*>(vertices.data()), vertices.size() * sizeof(Vertex));
	if (IsIndicesBuffer())
	{
		ibo = std::make_shared<IndexBuffer>(static_cast<const void*>(indices.data()), static_cast<uint32_t>(indices.size()));
	}

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, Vertex::POSITION_TYPE_DIMENSION);
	vbl.AddAttributeLayout(VertexAttributeLocation::Normal, GL_FLOAT, Vertex::NORMAL_TYPE_DIMENSION);
	vbl.AddAttributeLayout(VertexAttributeLocation::TextCoord, GL_FLOAT, Vertex::TEXCOORDS_TYPE_DIMENSION);
	vbl.AddAttributeLayout(VertexAttributeLocation::Tangent, GL_FLOAT, Vertex::TANGENT_TYPE_DIMENSION);
	vbl.AddAttributeLayout(VertexAttributeLocation::Bitangent, GL_FLOAT, Vertex::BITANGENT_TYPE_DIMENSION);
	vao->AddBuffer(std::move(vbl));

	// Do NOT unbind IBO before VAO since the latter contains references to IBO BindBuffer
	vao->Unbind();
	vbo.Unbind();
	if (IsIndicesBuffer())
	{
		ibo->Unbind();
	}
}

void MeshComponent::StoreInstanceModelMatrices(const VertexBuffer& vbo) const
{
	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol1, GL_FLOAT, Vertex::INSTANCE_MATRIX_TYPE_DIMENSION);
	vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol2, GL_FLOAT, Vertex::INSTANCE_MATRIX_TYPE_DIMENSION);
	vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol3, GL_FLOAT, Vertex::INSTANCE_MATRIX_TYPE_DIMENSION);
	vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol4, GL_FLOAT, Vertex::INSTANCE_MATRIX_TYPE_DIMENSION);
	vao->AddInstancedBuffer(std::move(vbl));
}

void MeshComponent::Render(const unsigned int mode) const
{
	// Do not call the same OpenGL wrapper function whether there is a non-null indices buffer
	if (IsIndicesBuffer())
	{
		// Expected to be called for any Mesh defining indices
		if (ibo == nullptr || ibo->GetCount() == 0)
		{
			std::cout << "ERROR::MESH - Any call to this method should have a non-null IBO!" << std::endl;
		}

		Renderer::Draw(*vao, mode, indices.size(), nullptr);
	}
	else
	{
		Renderer::Draw(*vao, mode, 0, vertices.size());
	}
}

void MeshComponent::RenderInstances(const uint32_t instanceCount) const
{
	Renderer::DrawInstances(*vao, GL_TRIANGLES, 0, ibo->GetCount(), instanceCount);
}