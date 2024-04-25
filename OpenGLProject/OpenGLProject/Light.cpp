#include "Light.h"

#include <glm/gtc/type_ptr.hpp>

#include "UniformBuffer.h"
#include "Utils.h"



Light::Light(const glm::vec3& inPosition,
	const glm::vec3& inAmbiant, const glm::vec3& inDiffuse, const glm::vec3& inSpecular,
	const float inConstant, const float inLinear, const float inQuadratic,
	const float inIsBlinn) : position(inPosition),
	ambiant(inAmbiant), diffuse(inDiffuse), specular(inSpecular),
	constant(inConstant), linear(inLinear), quadratic(inQuadratic),
	isBlinn(inIsBlinn)
{

}

void Light::Store(const std::vector<unsigned int>& entitiesShadersIDs)
{
	ubo = new UniformBuffer(entitiesShadersIDs, "lightParameters", static_cast<size_t>(4 * Utils::vec4Size + 4 * Utils::scalarSize));
	ubo->InitSubData({
		{ static_cast<const void*>(glm::value_ptr(position)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(ambiant)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(diffuse)), Utils::vec4Size },
		{ static_cast<const void*>(glm::value_ptr(specular)), Utils::vec4Size },
		{ static_cast<const void*>(&constant), Utils::scalarSize },
		{ static_cast<const void*>(&linear), Utils::scalarSize },
		{ static_cast<const void*>(&quadratic), Utils::scalarSize },
		{ static_cast<const void*>(&isBlinn), Utils::scalarSize }
		});
}