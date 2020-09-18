#include "pch.h"
#include "IndexBuffer.h"

RenderEngine::IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &_id);
}

void RenderEngine::IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void RenderEngine::IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void RenderEngine::IndexBuffer::Destroy()
{
	glDeleteBuffers(1, &_id);
}

void RenderEngine::IndexBuffer::BufferData(std::vector<unsigned int> indices)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void RenderEngine::IndexBuffer::BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
	glBufferData(target, size, data, usage);
}
