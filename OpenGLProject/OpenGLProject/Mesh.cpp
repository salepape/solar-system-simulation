#include "Mesh.h"

#include <glad.h>
#include <iostream>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"



Mesh::Mesh()
{

}

Mesh::Mesh(const std::vector<Vertex> verticesArg, const std::vector<unsigned int> indicesArg, const std::vector<Texture> texturesArg) :
	vertices(verticesArg), indices(indicesArg), textures(texturesArg)
{
	Compute();
	Store();
}

Mesh::~Mesh()
{

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
	vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
	
	if (indices.empty() == false)
	{
		ibo = new IndexBuffer(indices.data(), indices.size());
	}

	vao->Bind();

	if (indices.empty() == false)
	{
		ibo->Bind();
	}

	// how to read that position in Vertex struct is of 3 elmts with glm::vec3
	// Can we parse the shader to read the attribute locations?
	VertexBufferLayout vbl;
	vbl.AddAttributeLayout(VertexAttributeLocation::Position, GL_FLOAT, Vertex::GetPositionNumElmts());
	vbl.AddAttributeLayout(VertexAttributeLocation::Normal, GL_FLOAT, Vertex::GetNormalNumElmts());
	vbl.AddAttributeLayout(VertexAttributeLocation::TextCoord, GL_FLOAT, Vertex::GetTexCoordsNumElmts());
	vbl.AddAttributeLayout(VertexAttributeLocation::Tangent, GL_FLOAT, Vertex::GetTangentNumElmts());
	vbl.AddAttributeLayout(VertexAttributeLocation::Bitangent, GL_FLOAT, Vertex::GetBitangentNumElmts());
	vao->AddBuffer(*vbo, vbl);

	if (indices.empty() == false)
	{
		ibo->Unbind();
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

	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		textures[i].Enable(textureUnit);
	}

	renderer.Draw(*vao, *ibo);

	//glActiveTexture(GL_TEXTURE0);
}
