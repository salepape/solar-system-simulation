
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

	vao->Unbind();
}

void Mesh::Draw(Shader &shader)
{
	// Bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		// Active proper texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i);

		// Retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);		// transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);		// transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++);		// transfer unsigned int to stream

		// Set the sampler to the correct texture unit
		shader.setInt((name + number).c_str(), i);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].textID);
	}

	// Draw mesh
	vao->Bind();			
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	vao->Unbind();		

	glActiveTexture(GL_TEXTURE0);
}





//// set the vertex attribute pointers
//
//glEnableVertexAttribArray(0);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//
//glEnableVertexAttribArray(1);
//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//
//glEnableVertexAttribArray(2);
//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//
//glEnableVertexAttribArray(3);
//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//
//glEnableVertexAttribArray(4);
//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

// A great thing about structs is that their memory layout is sequential for all its items.
// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
// again translates to 3/2 floats which translates to a byte array.

	// always good practice to set everything back to defaults once configured.