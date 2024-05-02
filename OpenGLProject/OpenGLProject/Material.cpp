#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

#include "UniformBuffer.h"
#include "Utils.h"



Material::Material(const glm::vec3& inSpecular, const float inShininess) :
	specularColour(inSpecular), shininess(inShininess)
{

}

void Material::Store(const std::vector<uint32_t>& entitiesShadersIDs)
{
	ubo = new UniformBuffer(entitiesShadersIDs, "specularMatParams", Utils::vec4Size + Utils::scalarSize);
	ubo->InitSubData({
		{ static_cast<const void*>(glm::value_ptr(specularColour)), Utils::vec4Size },
		{ static_cast<const void*>(&shininess), Utils::scalarSize }
		});
}