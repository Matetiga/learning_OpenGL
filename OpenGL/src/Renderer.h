#pragma once

#include <GL/glew.h>

// Error handling
#define ASSERT(x) if (!(x)) __debugbreak();
// # before x will transform it into a string
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
