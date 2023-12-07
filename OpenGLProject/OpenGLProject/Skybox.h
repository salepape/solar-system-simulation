#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>

#include "Mesh.h"

class Renderer;
class Texture;



class Skybox : public Mesh
{
private:
	Texture* texture{ nullptr };

protected:
	void Compute() override;

public:
	Skybox(std::string path);
	~Skybox();

	void Render(const Renderer& renderer, const unsigned int& textureUnit) override;
};



#endif // SKYBOX_H