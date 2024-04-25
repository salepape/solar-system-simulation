#include "Material.h"

#include <glm/gtc/type_ptr.hpp>

#include "UniformBuffer.h"
#include "Utils.h"



Material::Material(const glm::vec3& inSpecular, const float inShininess) :
	specular(inSpecular), shininess(inShininess)
{

}

void Material::Store(const std::vector<unsigned int>& entitiesShadersIDs)
{
	ubo = new UniformBuffer(entitiesShadersIDs, "materialParameters", static_cast<size_t>(Utils::mat4v4Size + Utils::scalarSize));
	ubo->InitSubData({
		{ static_cast<const void*>(glm::value_ptr(specular)), Utils::vec4Size },
		{ static_cast<const void*>(&shininess), Utils::scalarSize }
		});
}