#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <glad.h>
#include <vector>



struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetTypeSize(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		default:
			return 0;
		}
	}
};

// 1st vector is vertex position (location = 0 in a Vertex Shader)
// 2nd vector is vertex normal (location = 1 in a Vertex Shader)
// 3rd vector is vertex texture coord (location = 2 in a Vertex Shader)
// 4th vector is vertex tangent (location = 3 in a Vertex Shader)
// 5th vector is vertex bitangent (location = 4 in a Vertex Shader)
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout();
	~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count)
	{

	}

	template <>
	void Push<float>(unsigned int count)
	{
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * VertexBufferElement::GetTypeSize(GL_FLOAT);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return elements; }

	inline unsigned int GetStride() const { return stride; }
};



#endif // VERTEXBUFFERLAYOUT_H