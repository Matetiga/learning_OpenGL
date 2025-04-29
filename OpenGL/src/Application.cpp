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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


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
    window = glfwCreateWindow(960, 540, "OpenGL", NULL, NULL);
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
            // the last two values of each row are the texture coordinate (values of 0 to 1.0) 
            // so each corner of the image is directly bound to each corner of the rect
            400.0f, 200.0f, 0.0f, 0.0f, // bottom left   --- 0 
            500.0f, 200.0f, 1.0f, 0.0f, // bottom rigth   --- 1
            500.0f, 300.0f, 1.0f, 1.0f,// top right        --- 2
            400.0f, 300.0f, 0.0f, 1.0f, // top left       --- 3
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
        // for the texture coordinates
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        // the four first values give the position of the window itself (to put the edges into coordinates) 
        // bottom left (0, 0)
        // top rigth (960, 540)
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.3, 0.5, 1, 0.8);



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



        // For ImGui 
        const char* glsl_version = "#version 300 es";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);


        //IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();


        ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init(glsl_version);
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        
        glm::vec3 translation(200, 200, 0);

        // Delta Time
        float delta_time = 0.16666f;
        auto start = std::chrono::high_resolution_clock::now();       

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model;

            

            shader.Bind();
            shader.SetUniform4f("u_Color", red, 0.5, 1, 0.8);
            shader.SetUniformMat4("u_MVP", mvp);
 
            renderer.Draw(va, ib, shader);

            if (red > 1.0f)
                increment = -0.05f;
            else if (red < 0.0f)
                increment = 0.05f;

            red += increment * delta_time * 10;

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }


            // Before we swap the buffers
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}