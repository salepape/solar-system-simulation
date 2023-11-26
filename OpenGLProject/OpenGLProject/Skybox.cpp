#include "Skybox.h"



Skybox::Skybox(const char* path)
{
	texture = new Texture(path, GL_TEXTURE_CUBE_MAP, GeometryType::CUBE, MapType::NONE);
	texture->LoadCubemapDDS();
	textures.push_back(*texture);

	Compute();
	Store();
};

void Skybox::Compute()
{
	std::vector<float> vertCoor =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	constexpr int numFaces = 6;
	constexpr int numVerticesPerFace = 6;
	int index = 0;

	for (int i = 0; i < numFaces; ++i)
	{
		for (int j = 0; j < numVerticesPerFace; ++j)
		{
			Vertex vertex;
			vertex.Position = glm::vec3(vertCoor[index], vertCoor[index + 1], vertCoor[index + 2]);
			vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
			vertex.TexCoords = glm::vec3(0.0f, 0.0f, 0.0f);
			vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
			vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
			vertices.push_back(vertex);

			index += vertex.Position.length();
		}
	}
}

Skybox::~Skybox()
{

}

void Skybox::Render(const Renderer& renderer, const unsigned int& textureUnit)
{
	texture->Enable(textureUnit);
	renderer.Draw(*vao, GL_TRIANGLES, 36);
}

