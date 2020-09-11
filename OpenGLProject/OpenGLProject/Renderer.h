#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"



class Renderer
{
private:
public:
	void Clear() const;
	void DepthTest() const;
	void DepthClamp() const;
	void Blend() const;
	void CullFace() const;
	void DepthEqual() const;
	void DepthLess() const;
	void SetColor();

	void DrawInstanced(const VertexArray& vao, unsigned int iboCount, unsigned int count) const;
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, unsigned int mode, unsigned int count) const;
	void Draw(const VertexArray& vao, const IndexBuffer& ibo) const;
	void Draw(const VertexArray& vao, unsigned int mode, unsigned int count) const;
};

