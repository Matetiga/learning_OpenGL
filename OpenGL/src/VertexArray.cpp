#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_rendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // we have to bind the VertexArray 
    Bind();
    // and the VertexBuffer 
	vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i)); // enables vertex attribute at index i

        // OpenGL links the currently bound VBO (vb) to the vertex attribute (index i) in the currently bound VAO 
        // the first value (index) matches with the enabled vertex
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }


}


void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_rendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}