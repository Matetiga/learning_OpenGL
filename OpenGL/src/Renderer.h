#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


// Error handling
#define ASSERT(x) if (!(x)) __debugbreak();
// # before x will transform it into a string
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);



class Renderer
{
public:
    const void Clear();
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};