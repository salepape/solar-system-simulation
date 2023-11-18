#include "Mesh.h"



Mesh::Mesh(const std::vector<Vertex> verticesArg, const std::vector<unsigned int> indicesArg, const std::vector<Texture> texturesArg):
	vertices(verticesArg), indices(indicesArg), textures(texturesArg)
{
	// Set vertex buffers and its attribute pointers once we have all required data
	Store();
}

void Mesh::Store()
{
	vao = new VertexArray();
	vbo = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
	ibo = new IndexBuffer(&indices[0], indices.size());

	vao->Bind();
	ibo->Bind();

	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vao->AddBuffer(*vbo, vbl);

	//ibo->Unbind();
	vao->Unbind();
}

Mesh::~Mesh()
{

}

void Mesh::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	for (unsigned int i = 0; i < textures.size(); ++i)
		textures[i].Enable(textureUnit);

	renderer.Draw(*vao, *ibo);

	//glActiveTexture(GL_TEXTURE0);
}
