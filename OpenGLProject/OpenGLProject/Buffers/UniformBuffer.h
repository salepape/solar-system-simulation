#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <cstddef> // std::size_t
#include <cstdint>
#include <string>
#include <vector>

#include "DataBuffer.h"
#include "Rendering/ShaderLoader.h"



// Define a set of fields contained in a GLSL struct (defined via 'layout' keyword) shared across several GLSL Vertex/Fragment Shaders
class UniformGLSLStruct
{
public:
	void AddUniformField(const void* data, const std::size_t type) { uniformFields.emplace_back(SubData{ data, type }); }

	// Compute total size of the struct (corresponding to the sum of each GLSL variable size in the case of structs)
	std::size_t GetSizeInBytes() const
	{
		std::size_t result = 0;
		for (const SubData& uniformField : uniformFields)
		{
			result += uniformField.sizeInBytes;
		}

		return result;
	}

	const std::vector<SubData>& GetUniformFields() const { return uniformFields; }

private:
	std::vector<SubData> uniformFields;
};

// Store the reference to a GLSL Uniform that is shared across several GLSL Vertex/Fragment Shaders, to allow for a single call to update its value in all of them
// Note: maximise its use, as it has a non-negligeable impact on performance
class UniformBuffer : public DataBuffer
{
public:
	// @todo - Could be automated when reading shader text file
	// inGLSLUniformName - Can either be a single variable or a set of variables stored in struct defined using the 'layout (std140)' syntax in a GLSL Vertex/Fragment Shader
	UniformBuffer(const std::string& inGLSLUniformName, const GLSLUniform::Enum inGLSLUniform);

	void SetData(const void* data, const std::size_t sizeInBytes);
	void SetData(const UniformGLSLStruct& inStruct);

private:
	uint32_t blockBindingPoint{ 0 };
	static uint32_t globalBlockBindingPoint;
};



#endif // UNIFORM_BUFFER_H