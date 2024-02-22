#ifndef SPHERE_H
#define SPHERE_H

#include <string>

#include "Mesh.h"



class Sphere: public Mesh
{
public:
	Sphere(std::string inTexturePath, const float inRadius, const int inMeridianStripsCount = 100, const int inParallelStripsCount = 100);
	~Sphere();

protected:
	// Note that we call square the zone created by the intersection between one meridian strip and one parallel strip
	void Compute() override;

private:
	float radius{ 0.0f };
	int meridianStripsCount{ 0 };
	int parallelStripsCount{ 0 };
};



#endif // SPHERE_H