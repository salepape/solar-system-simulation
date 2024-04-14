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

Mesh::Mesh(const std::string& texturePath, int textureType, GeometryType geometryType, MapType mapType)
{
	Texture texture(texturePath, textureType, geometryType, mapType);
	texture.LoadDDS();
	textures.push_back(texture);
}

Mesh::Mesh(const std::vector<Vertex>& inVertices, const std::vector<unsigned int>& inIndices, const std::vector<Texture>& inTextures) :
	vertices(inVertices), indices(inIndices), textures(inTextures)
{
	ComputeVertices();
	StoreVertices();
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

void Mesh::ComputeVertices()
{

}

void Mesh::StoreVertices()
{
	if (vertices.empty())
	{
		std::cout << "ERROR::MESH - No vertices found!" << std::endl;
		return;
	}
	const bool isIndexBuffer = indices.empty() == false;

	vao = new VertexArray();

	VertexBuffer vbo(static_cast<const void*>(vertices.data()), static_cast<unsigned int>(vertices.size()) * sizeof(Vertex));

	if (isIndexBuffer)
	{
		ibo = new IndexBuffer(static_cast<const void*>(indices.data()), static_cast<unsigned int>(indices.size()));
	}

	vao->Bind();

	if (isIndexBuffer)
	{
		ibo->Bind();
	}

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, Vertex::POSITION_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Normal, GL_FLOAT, Vertex::NORMAL_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::TextCoord, GL_FLOAT, Vertex::TEXCOORDS_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Tangent, GL_FLOAT, Vertex::TANGENT_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Bitangent, GL_FLOAT, Vertex::BITANGENT_ELMTS_COUNT);
	vao->AddBuffer(vbo, vbl);

	if (isIndexBuffer)
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

	for (const auto& texture: textures)
	{
		texture.Enable(textureUnit);
	}

	renderer.Draw(*vao, *ibo);

	//glActiveTexture(GL_TEXTURE0);
}