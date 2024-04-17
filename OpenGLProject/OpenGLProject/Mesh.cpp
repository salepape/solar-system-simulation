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

Mesh::Mesh(const std::string& texturePath, const int textureTarget, const unsigned int textureWrapOption, const unsigned int textureFilterOption, const TextureType textureType)
{
	Texture texture(texturePath, textureTarget, textureWrapOption, textureFilterOption, textureType);
	texture.LoadDDS();
	textures.push_back(texture);
}

Mesh::Mesh(const std::vector<Vertex>& inVertices, const std::vector<unsigned int>& inIndices, const std::vector<Texture>& inTextures) :
	vertices(inVertices), indices(inIndices), textures(inTextures)
{
	StoreVertices();
}

Mesh::~Mesh()
{
	//if (vao)
	//{
	//	delete vao;
	//}
	//
	//if (ibo)
	//{
	//	delete ibo;
	//}
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
	VertexBuffer vbo(static_cast<const void*>(vertices.data()), static_cast<size_t>(vertices.size()) * sizeof(Vertex));
	if (isIndexBuffer)
	{
		ibo = new IndexBuffer(static_cast<const void*>(indices.data()), static_cast<unsigned int>(indices.size()));
	}

	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, Vertex::POSITION_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Normal, GL_FLOAT, Vertex::NORMAL_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::TextCoord, GL_FLOAT, Vertex::TEXCOORDS_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Tangent, GL_FLOAT, Vertex::TANGENT_ELMTS_COUNT);
	vbl.AddAttributeLayout(VertexAttributeLocation::Bitangent, GL_FLOAT, Vertex::BITANGENT_ELMTS_COUNT);
	vao->AddBuffer(vbo, vbl);

	// Do NOT unbind IBO before VAO since the latter contains references to IBO BindBuffer
	vao->Unbind();
	vbo.Unbind();
	if (isIndexBuffer)
	{
		ibo->Unbind();
	}
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