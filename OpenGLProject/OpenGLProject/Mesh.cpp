#include "Mesh.h"



Mesh::Mesh(std::vector<Vertex> verticesArg, std::vector<unsigned int> indicesArg, std::vector<Texture> texturesArg):
	vertices(verticesArg), indices(indicesArg), textures(texturesArg)
{
	// Set vertex buffers and its attribute pointers once we have all required data
	Store();
}

void Mesh::Store()
{
	// Create buffers / arrays
	vao = new VertexArray();
	vbo = new VertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex));
	ibo = new IndexBuffer(&indices[0], indices.size());

	vao->Bind();
	ibo->Bind();

	VertexBufferLayout vbl;
	vbl.Push<float>(3);			// vertex position
	vbl.Push<float>(3);			// vertex normal
	vbl.Push<float>(2);			// vertex texture coord
	vbl.Push<float>(3);			// vertex tangent
	vbl.Push<float>(3);			// vertex bitangent
	vao->AddBuffer(*vbo, vbl);

	//ibo->Unbind();
	vao->Unbind();
}

Mesh::~Mesh()
{

}

void Mesh::Render(Renderer& renderer, unsigned int& textureUnit)
{
	for (unsigned int i = 0; i < textures.size(); ++i)
		textures[i].Enable(textureUnit);

	renderer.Draw(*vao, *ibo);

	//glActiveTexture(GL_TEXTURE0);
}
