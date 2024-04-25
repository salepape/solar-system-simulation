#ifndef LIGHT_H
#define LIGHT_H

#include <glm/vec3.hpp>
#include <vector>

class UniformBuffer;



class Light
{
public:
	Light() = default;
	Light(const glm::vec3& inPosition,
		const glm::vec3& inAmbiant, const glm::vec3& inDiffuse, const glm::vec3& inSpecular,
		const float inConstant, const float inLinear, const float inQuadratic,
		const float inIsBlinn);

	void Store(const std::vector<unsigned int>& entitiesShadersIDs);

private:
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };

	// Terms of Phong shading formula 
	glm::vec3 ambiant{ 0.2f, 0.2f, 0.2f };
	glm::vec3 diffuse{ 0.95f, 0.95f, 0.95f };
	glm::vec3 specular{ 1.0f, 1.0f, 1.0f };

	// Terms of attenuation spotlight formula
	float constant{ 1.0f };
	float linear{ 0.0007f };
	float quadratic{ 0.000002f };

	bool isBlinn{ false };

	UniformBuffer* ubo{ nullptr };
};



#endif // LIGHT_H