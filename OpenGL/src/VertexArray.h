#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray 
{
private:
	unsigned int m_rendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddColorBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int location);


	void Bind() const;
	void Unbind() const;
};