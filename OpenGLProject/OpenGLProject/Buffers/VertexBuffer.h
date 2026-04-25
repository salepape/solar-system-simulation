#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

#include <cstddef> // std::size_t

#include "DataBuffer.h"



// Block of contiguous memory which holds plain Vertex data. Does not have to be defined per Model/Mesh if more performant.
// Its layout in GLSL Shaders correspond to Vertex Attributes, and need to be stored in a VAO preferrably to be in line with modern OpenGL.
// Note: binding of VBOs is a global OpenGL state, and decide when subsequent OpenGL calls will work
class VertexBuffer : public DataBuffer
{
public:
	// Constructor of VBO includes a binding and a data setter, so no need to call these methods separately just after instantiation
	VertexBuffer(const void* data, const std::size_t sizeInBytes, const int usage = GL_STATIC_DRAW);
};



#endif // VERTEX_BUFFER_H