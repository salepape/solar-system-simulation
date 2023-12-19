#ifndef SPHERE_H
#define SPHERE_H

#include <string>

#include "Mesh.h"



class Sphere: public Mesh
{
public:
	Sphere(std::string texturePath, const float radiusArg);
	~Sphere();

protected:
	// Note that we call square the zone created by the intersection between one meridian strip and one parallel strip
	void Compute() override;

private:
	float radius{ 0.0f };
};



#endif // SPHERE_H