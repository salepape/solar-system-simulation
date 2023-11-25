#include "Mesh.h"



Mesh::Mesh()
{

}

Mesh::Mesh(const std::vector<Vertex> verticesArg, const std::vector<unsigned int> indicesArg, const std::vector<Texture> texturesArg) :
	vertices(verticesArg), indices(indicesArg), textures(texturesArg)
{
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
	vbo = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
	
	if (indices.empty() == false)
	{
		ibo = new IndexBuffer(&indices[0], indices.size());
	}

	vao->Bind();

	if (indices.empty() == false)
	{
		ibo->Bind();
	}

	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	vbl.Push<float>(3);
	vbl.Push<float>(3);
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
