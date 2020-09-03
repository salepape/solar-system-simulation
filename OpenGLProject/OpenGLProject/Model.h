#ifndef MODEL_H
#define MODEL_H

//#include "stb_image.h"
#include<SOIL2.h>

#include <iostream>
//#include <string>
//#include <vector>
#include <map>

#include "mesh.h"	



class Model
{
private:
	// Load a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector
	void LoadModel(std::string const &path);

	// Process a node in a recursive fashion (ie. each individual mesh located at the node and repeats this process on its children nodes (if any))
	void ProcessNode(aiNode *node, const aiScene *scene);

	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

	// Check all material textures of a given type and loads the textures if they're not loaded yet
	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

public:
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh>    meshes;
	std::string directory;
	bool gammaCorrection;

	Model(std::string const &path, bool gammaCorrectionArg = false);

	void Draw(ShaderProgram &shader);
};

#endif