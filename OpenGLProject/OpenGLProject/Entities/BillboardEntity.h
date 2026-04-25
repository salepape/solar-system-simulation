#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <glm/vec3.hpp>

#include <string>

#include "Components/Meshes/QuadMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "SceneEntity.h"

struct BodyData;
struct GlyphParams;



// Create set of quads according to FreeType glyph texture and parameters retrieved out of the Glyph Loader
class BillboardEntity : public SceneEntity
{
public:
	BillboardEntity(const BodyData& inBodyData);

	void Render(const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight);

private:
	std::string legend;
	bool isMoon{ false };

	// Reduce default glyph texture size we get out of FreeType, as it's too big compared to celestial body sizes
	float glyphAdvanceScaleFactor{ 1.0f };

	QuadMeshComponent quads;
	BlinnPhongMaterial material;

	BlinnPhongMaterial InitialiseMaterial();

	void ComputeModelMatrixVUniform(const float /*elapsedTime*/) override {};
	void ComputeModelMatrixVUniform(const glm::vec3& bodyPosition, const glm::vec3& cameraForward, const glm::vec3& cameraRight);

	// Sum up all the glyph advance values, as per FreeType convention (i.e. the width needed each glyph to be rendered, inclusive of horizontal spacing)
	float ComputeBillboardWidth() const;

	// Get the advance value of a glyph, as per FreeType convention (i.e. the width needed each glyph to be rendered, inclusive of horizontal spacing)
	float GetGlyphAdvance(const GlyphParams& c) const;

	// Return a vector of params for each quad (one for each glyph) to be rendered on this billboard
	std::vector<QuadParams> ComputeQuadParams(const float billboardXStart, const float billboardYStart);
};



#endif // BILLBOARD_H