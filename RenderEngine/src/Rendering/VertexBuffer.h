#pragma once

#include "GL/glew.h"

namespace RenderEngine {

    class VertexBuffer
    {
    public:
        VertexBuffer();

        void Bind();
        void Unbind();
        void Destroy();

        void BufferData(std::vector<float> vertices);
        void BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

        void EnableAttributeArray(int attr);
        void DisableAttributeArray(int attr);
        void AttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);
        void AttributePointer(GLuint index, GLint size, const void* offset);
        void AttributePointer(GLint size, const void* offset);
        void AttributePointer(GLint size);

        void BufferSubData(std::vector<float> vertices);
        void BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
        void NamedBufferSubData(std::vector<float> vertices);
        void NamedBufferSubData(GLintptr offset, GLsizeiptr size, const void* data);

        void* MapBuffer();
        void UnMapBuffer();

        inline GLuint GetId() { return _id; }
    private:
        GLuint _id = -1;

    };

}

