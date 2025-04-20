#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // the buffer swap occurs only once per monitor refresh cycle

    // this must be called after a creating a valid OpenGL context
    if (glewInit() != GLEW_OK) {
        std::cout << "Error with glew";
    }

    std::cout << " GL version : " << glGetString(GL_VERSION) << std::endl;


    {
        float position[] = {
            -0.5f, -0.5f, // bottom left   --- 0 
            0.5f, -0.5f, // bottom rigth   --- 1
            0.5f, 0.5f,// top right        --- 2
            -0.5f, 0.5f, // top left       --- 3
        };

        // Index Buffer -> to avoid re rendering the same vertex twice
        unsigned int indices[]{ // ANY INDEX BUFFER must be UNSIGNED
            0, 1, 2,
            2, 3, 0
        };

        /* IMPORTANT !!
         OpenGL is a state machine
         this means the sequence of lines of code play an immense role
         some actions can only be executed if a previous line was called
        */

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(position, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);


        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.3, 0.5, 1, 0.8);

        // unbind the buffers
        va.Unbind();
        shader.Unbind();        
        vb.Unbind();
        ib.Unbind();
        
        // Color of the form
        float red = 0.0f;
        float increment = 0.05f;

        // Delta Time
        float delta_time = 0.16666f;
        auto start = std::chrono::high_resolution_clock::now();       

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", red, 0.5, 1, 0.8);
            

            // we are just binding the index array buffer and the vertex array object and NOT the position's buffer (vertex buffer)
            //GLCall(glBindVertexArray(vao));
            va.Bind();
            ib.Bind();


            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (red > 1.0f)
                increment = -0.05f;
            else if (red < 0.0f)
                increment = 0.05f;

            red += increment * delta_time * 10;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            
            
            // Delta Time Calc
            auto end = std::chrono::high_resolution_clock::now();
            delta_time = std::chrono::duration<float>(end - start).count();
            start = end;
        }

        // at the end of this scope, IndexBuffer will be deleted (Stack allocated)
        // preventing infinit loop of glGetError() 
        // the Buffers can also be heap allocated
    }
    glfwTerminate();
    return 0;
}