#include "Mesh.h"

#include <glad.h>
#include <iostream>
#include <utility>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



Mesh::Mesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices) :
	vertices(inVertices), indices(inIndices)
{
	StoreVertices();
}

void Mesh::StoreVertices()
{
	if (vertices.empty())
	{
		std::cout << "ERROR::MESH - No vertices found!" << std::endl;
		return;
	}
	const bool isIndexBuffer = indices.empty() == false;

	vao = std::make_shared<VertexArray>();
	VertexBuffer vbo(static_cast<const void*>(vertices.data()), vertices.size() * sizeof(Vertex));
	if (isIndexBuffer)
	{
		ibo = std::make_shared<IndexBuffer>(static_cast<const void*>(indices.data()), static_cast<uint32_t>(indices.size()));
	}

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, Vertex::POSITION_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Normal, GL_FLOAT, Vertex::NORMAL_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::TextCoord, GL_FLOAT, Vertex::TEXCOORDS_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Tangent, GL_FLOAT, Vertex::TANGENT_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Bitangent, GL_FLOAT, Vertex::BITANGENT_ELMTS_COUNT);
	vao->AddBuffer(vbo, std::move(vbl));

	// Do NOT unbind IBO before VAO since the latter contains references to IBO BindBuffer
	vao->Unbind();
	vbo.Unbind();
	if (isIndexBuffer)
	{
		ibo->Unbind();
	}
}

void Mesh::StoreInstanceModelMatrices(const VertexBuffer& vbo) const
{
	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol1, GL_FLOAT, Vertex::INSTANCE_MATRIX_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol2, GL_FLOAT, Vertex::INSTANCE_MATRIX_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol3, GL_FLOAT, Vertex::INSTANCE_MATRIX_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::InstancedMatrixCol4, GL_FLOAT, Vertex::INSTANCE_MATRIX_ELMTS_COUNT);
	vao->AddInstancedBuffer(vbo, std::move(vbl));
}

void Mesh::Render(const Renderer& renderer) const
{
	if (indices.empty())
	{
		std::cout << "ERROR::MESH - The base version of Render() needs indices array to be non null!" << std::endl;
		return;
	}

	renderer.Draw(*vao, *ibo);
}

void Mesh::RenderInstances(const Renderer& renderer, const uint32_t instanceCount) const
{
	renderer.DrawInstances(*vao, ibo->GetCount(), instanceCount);
}