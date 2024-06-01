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
	Material(Shader& inShader);
	Material(Shader& inShader, const glm::vec3& inSpecular, const float inShininess, const float inTransparency = 1.0f);
	Material(Material&& inMaterial);

	void SetDiffuseSamplerVUniform();
	void SetDiffuseColourVUniform(const glm::vec3& colour);

	Shader& GetShader() { return shader; }
	uint32_t GetDiffuseSamplerID() const { return diffuseSamplerID; }

private:
	// We need a reference so the shader persists even after Material destruction (Shader exists separately in ResourceLoader)
	Shader& shader;

	// Diffuse texture sampler ID (one for each object)
	static uint32_t diffuseSamplerIDCounter;
	uint32_t diffuseSamplerID{ 0 };
	
	// @todo - Move all the corresponding Texture code in this class
	// DDS texture that will be used as a Sampler2D
	//Texture diffuseTexture;

	glm::vec3 diffuseColour{ 0.0f, 0.0f, 0.0f };

	// Colour of the specular highlight on the surface (instead of a texture)
	glm::vec3 specularColour{ 0.0f, 0.0f, 0.0f };

	// Coefficient altering the radius of the specular highlight
	float shininess{ 64.0f };

	// Coefficient corresponding to the alpha value in a colour vector
	float transparency{ 1.0f };



	void SetFUniforms();
};



#endif // MATERIAL_H