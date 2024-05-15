#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "UniformBuffer.h"

class Shader;



// Material properties of the surface
class Material
{
public:
	Material() = default;
	Material(const glm::vec3& inSpecular, const float inShininess, const float inTransparency = 1.0f);

	void Store(const std::vector<uint32_t>& entitiesShadersIDs);
	void SetDiffuseSamplerUniform(Shader& shader, const uint32_t samplerID);

private:
	// DDS texture that will be used as a Sampler2D
	// @todo - Find ways to put the equivalent uniform out of the Render loop with this pointer?
	//std::unique_ptr<Texture> diffuseTexture;

	// @todo - Check code again, as I'm not under the impression the variables below work correctly...

	// Colour of the specular highlight on the surface (instead of a texture)
	glm::vec3 specularColour{ 0.0f, 0.0f, 0.0f };

	// Coefficient altering the radius of the specular highlight
	float shininess{ 64.0f };

	// Coefficient corresponding to the alpha value in a colour vector
	// @todo - Make this parameter part of the UBO to avoid having a separate uniform variable once the Material system is implemented
	float transparency{ 1.0f };

	std::unique_ptr<UniformBuffer> ubo;
};



#endif // MATERIAL_H