#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

#include "Mesh.h"



class Sphere: public Mesh
{
private:
	float radius{ 0.0f };

protected:
	// Note that we call square the zone created by the intersection between one meridian strip and one parallel strip
	void Compute() override;

public:
	Sphere(const char* texturePath, const float radiusArg);
	~Sphere();
};



#endif // SPHERE_H
