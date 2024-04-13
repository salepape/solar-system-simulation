#include "Mesh.h"

#include <glad.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



Mesh::Mesh()
{

}

Mesh::Mesh(const std::vector<Vertex>& inVertices, const std::vector<unsigned int>& inIndices, const std::vector<Texture>& inTextures) :
	vertices(inVertices), indices(inIndices), textures(inTextures)
{
	Compute();
	Store();
}

Mesh::~Mesh()
{
	//if (ibo)
	//{
	//	ibo->~IndexBuffer();
	//}

	//if (vbo)
	//{
	//	vbo->~VertexBuffer();
	//}

	//if (vao)
	//{
	//	vao->~VertexArray();
	//}
}

void Mesh::Compute()
{

}

void Mesh::Store()
{
	if (vertices.empty())
	{
		std::cout << "ERROR::MESH - No vertices found!" << std::endl;
		return;
	}

	vao = new VertexArray();
	vbo = new VertexBuffer(vertices.data(), static_cast<unsigned int>(vertices.size()) * sizeof(Vertex));
	
	if (indices.empty() == false)
	{
		ibo = new IndexBuffer(indices.data(), static_cast<const unsigned int>(indices.size()));
	}

	vao->Bind();

	if (indices.empty() == false)
	{
		ibo->Bind();
	}

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, Vertex::GetPositionElmtsCount());
	vbl.AddAttributeLayout(VertexAttributeLocation::Normal, GL_FLOAT, Vertex::GetNormalElmtsCount());
	vbl.AddAttributeLayout(VertexAttributeLocation::TextCoord, GL_FLOAT, Vertex::GetTexCoordsElmtsCount());
	vbl.AddAttributeLayout(VertexAttributeLocation::Tangent, GL_FLOAT, Vertex::GetTangentElmtsCount());
	vbl.AddAttributeLayout(VertexAttributeLocation::Bitangent, GL_FLOAT, Vertex::GetBitangentElmtsCount());
	vao->AddBuffer(*vbo, vbl);

	if (indices.empty() == false)
	{
		// @todo - Figure out why not commenting this line screw up instancing
		//ibo->Unbind();
	}

	vao->Unbind();
}

void Mesh::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	if (indices.empty())
	{
		std::cout << "ERROR::MESH - The base version of Render() needs indices array to be non null!" << std::endl;
		return;
	}

	for (auto& texture: textures)
	{
		texture.Enable(textureUnit);
	}

	renderer.Draw(*vao, *ibo);

	//glActiveTexture(GL_TEXTURE0);
}