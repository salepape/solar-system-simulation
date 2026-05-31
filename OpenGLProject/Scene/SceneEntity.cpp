#include "SceneEntity.h"

uint32_t SceneEntity::entityIDCounter = 1;



SceneEntity::SceneEntity(const std::string& inName) :
	ID(entityIDCounter++),
	name(inName)
{

}
