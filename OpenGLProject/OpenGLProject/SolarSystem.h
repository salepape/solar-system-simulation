#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <vector>

#include "Belt.h"
#include "BodyRings.h"
#include "BodySystem.h"
#include "MilkyWay.h"
#include "Scene.h"

class Spacecraft;



// All member containers of this class are filled using the ResourceLoader
class SolarSystem : public Scene
{
public:
	SolarSystem();

	void Update() override;

	static std::vector<BodySystem>& GetBodySystemsVector();
	static std::vector<BodyRings>& GetRingsVector();
	static std::vector<Belt>& GetBeltsVector();

private:
	static std::vector<BodySystem> bodySystems;
	static std::vector<BodyRings> rings;
	static std::vector<Belt> belts;

	MilkyWay milkyWay;

	std::shared_ptr<Spacecraft> spacecraft;
};



#endif // SOLAR_SYSTEM_H