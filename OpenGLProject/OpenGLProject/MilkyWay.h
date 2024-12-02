#ifndef MILKY_WAY_H
#define MILKY_WAY_H

#include <filesystem>
#include <string>

#include "Material.h"
#include "SceneEntity.h"
#include "Skybox.h"

class Renderer;



class MilkyWay : public SceneEntity
{
public:
	MilkyWay(const std::filesystem::path& inTexturePath);

	void Render(const Renderer& renderer, const float elapsedTime = 0.0f) override;

private:
	Skybox skybox;

	static Material InitialiseParent(const std::filesystem::path& inTexturePath);

	void ComputeModelMatrixVUniform(const float elapsedTime = 0.0f) override {};
};



#endif // MILKY_WAY_H