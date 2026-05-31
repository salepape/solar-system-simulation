#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "Components/Meshes/QuadMeshComponent.h"
#include "Rendering/BlinnPhongMaterial.h"
#include "Scene/SceneEntity.h"

struct BodyData;
class Camera;
struct GlyphParams;



// Create set of quads according to FreeType glyph texture and parameters retrieved out of the Glyph Loader
class BillboardEntity : public SceneEntity, public ITransformable, public IRenderable
{
public:
	BillboardEntity(const BodyData& inBodyData);

	// IRenderable implementation
	BlinnPhongMaterial InitialiseMaterial(const std::filesystem::path& texturePath) /*override*/;
	void Render() override;
	// IRenderable implementation

private:
	std::string legend;

	// Reduce default glyph texture size we get out of FreeType, as it's too big compared to celestial body sizes
	float glyphTextureScaleFactor{ 1.0f };

	QuadMeshComponent quads;
	BlinnPhongMaterial material;

	// ITransformable implementation
	void ComputeModelMatrixVUniform(const float deltaTime, const Camera& camera, std::optional<std::reference_wrapper<const SceneEntity>> parentEntity = std::nullopt) override;
	// ITransformable implementation

	// Sum up all the glyph advance values, as per FreeType convention (i.e. the width needed each glyph to be rendered, inclusive of horizontal spacing)
	float ComputeBillboardWidth() const;

	// Get the advance value of a glyph, as per FreeType convention (i.e. the width needed each glyph to be rendered, inclusive of horizontal spacing)
	float GetGlyphAdvance(const GlyphParams& glyphParams) const;

	// Return a vector of params for each quad (one for each glyph) to be rendered on this billboard
	std::vector<QuadParams> ComputeQuadParams(const float billboardXStart, const float billboardYStart);
};



#endif // BILLBOARD_H
