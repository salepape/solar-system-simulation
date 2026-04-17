#include "UniformBuffer.h"

#include <glad/glad.h>

#include "Rendering/Shader.h"

uint32_t UniformBuffer::globalBlockBindingPoint = 0;

// @todo - To be move outside of "Engine code"
std::unordered_map<UniformShaderGroup, std::vector<ShaderLookUpID::Enum>> UniformBuffer::shaderGroups
{
	{
		UniformShaderGroup::PROJECTION_VIEW,
		{
			ShaderLookUpID::Enum::DEFAULT,
			ShaderLookUpID::Enum::SUN,
			ShaderLookUpID::Enum::BILLBOARD,
			ShaderLookUpID::Enum::BELT,
			ShaderLookUpID::Enum::MILKY_WAY,
		}
	},
	{
		UniformShaderGroup::LINE_OF_SIGHT,
		{
			ShaderLookUpID::Enum::DEFAULT,
			ShaderLookUpID::Enum::BELT,
		}
	}
};



UniformBuffer::UniformBuffer(const std::string& inGLSLUniformName, const UniformShaderGroup inShaderGroup)
{
	target = GL_UNIFORM_BUFFER;

	blockBindingPoint = globalBlockBindingPoint++;

	for (const ShaderLookUpID::Enum shaderLookUpID : shaderGroups[inShaderGroup])
	{
		const Program& shader = ShaderLoader::GetShader(shaderLookUpID);
		const uint32_t shaderID = shader.GetRendererID();

		// @todo - uniformBlockIndex = 6 for BELT Fragment Shader whereas it equals 7 for all other ones in LINE_OF_SIGHT group for camera position Uniform!
		const uint32_t uniformBlockIndex = glGetUniformBlockIndex(shaderID, inGLSLUniformName.c_str());
		glUniformBlockBinding(shaderID, uniformBlockIndex, blockBindingPoint);
	}

	// Reserve an ID available to be used by the UBO as a binding point
	glGenBuffers(1, &rendererID);
}

void UniformBuffer::SetData(const void* data, const std::size_t sizeInBytes)
{
	Bind();

	// Allocate memory space (in bytes) to the UBO and store data in it
	glBufferData(target, sizeInBytes, nullptr, GL_STATIC_DRAW);

	// Define the range of the buffer that is linked to the specified uniform binding point
	glBindBufferRange(target, blockBindingPoint, rendererID, 0, sizeInBytes);

	SetSubData(data, sizeInBytes);

	Unbind();
}

void UniformBuffer::SetData(const UniformGLSLStruct& inStruct)
{
	Bind();

	const std::size_t structSizeInBytes = inStruct.GetSizeInBytes();

	// Allocate memory space (in bytes) to the UBO and store data in it
	glBufferData(target, structSizeInBytes, nullptr, GL_STATIC_DRAW);

	// Define the range of the buffer that is linked to the specified uniform binding point
	glBindBufferRange(target, blockBindingPoint, rendererID, 0, structSizeInBytes);

	SetSubData(inStruct.GetUniformFields());

	Unbind();
}