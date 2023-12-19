#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>

#include "Mesh.h"

class Renderer;
class Texture;



class Skybox : public Mesh
{
public:
	Skybox(std::string path);
	~Skybox();

	void Render(const Renderer& renderer, const unsigned int& textureUnit) override;

protected:
	void Compute() override;

private:
	Texture* texture{ nullptr };
};



#endif // SKYBOX_H