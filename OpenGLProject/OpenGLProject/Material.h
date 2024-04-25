#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <vector>

class UniformBuffer;



class Material
{
public:
	Material() = default;
	Material(const glm::vec3& inSpecular, const float inShininess);

	void Store(const std::vector<unsigned int>& entitiesShadersIDs);

private:
	glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
	float shininess{ 64.0f };

	UniformBuffer* ubo{ nullptr };
};



#endif // MATERIAL_H