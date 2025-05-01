#include "Renderer.h"

#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGl Error] --> (" << error << ") function: " << function << " file : " << file <<
            " on line : " << line << std::endl;
        return false;
    }
    return true;
}

const void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    shader.Bind();
    // we are just binding the index array buffer and the vertex array object and NOT the position's buffer (VertexBuffer)
    // Vertex Array contains Information about the VertexBuffer and it's layout (position of the vertices and how to read them 
    // and texture coordinates)
    va.Bind();
    // Contains the Index of each Vertex and how to assemble everything together
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}
