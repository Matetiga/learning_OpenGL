#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; // to keep track of every Buffer/Object

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};