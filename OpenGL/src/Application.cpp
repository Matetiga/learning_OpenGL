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
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


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
            // the last two values of each row are the texture coordinate
            -0.5f, -0.5f, 0.0f, 0.0f, // bottom left   --- 0 
            0.5f, -0.5f, 1.0f, 0.0f, // bottom rigth   --- 1
            0.5f, 0.5f, 1.0f, 1.0f,// top right        --- 2
            -0.5f, 0.5f, 0.0f, 1.0f, // top left       --- 3
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

        // Blending 
        GLCall(glEnable(GL_BLEND));
        // glBlendFunc(src, dest) will specify the values with the src and destination's RGBA will be multiplied 
        // GL_SRC_ALPHA will be = 0, if the pixel should be transluscent (for png images)
        // dest 'GL_ONE_MINUS_SRC_ALPHA' 
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
        VertexArray va;
        VertexBuffer vb(position, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        // the four first values give the position of the window itself (to put the edges into coordinates)
        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);


        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.3, 0.5, 1, 0.8);
        shader.SetUniformMat4("u_MVP", proj);


        Texture texture("res/textures/nerd.png");
        texture.Bind(0); // this must match with the texture slot
        shader.SetUniform1i("u_Texture", 0); // Texture is bound to slot 0

        // unbind the buffers
        va.Unbind();
        shader.Unbind();        
        vb.Unbind();
        ib.Unbind();
        
        // Color of the form
        float red = 0.0f;
        float increment = 0.05f;

        Renderer renderer;

        // Delta Time
        float delta_time = 0.16666f;
        auto start = std::chrono::high_resolution_clock::now();       

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", red, 0.5, 1, 0.8);
 
            renderer.Draw(va, ib, shader);

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