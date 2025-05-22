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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestTriangle.h"
#include "tests/TestCube.h"

void masterKeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << "master key" << std::endl;
    test::Test* currentTest = static_cast<test::Test*>(glfwGetWindowUserPointer(window));
    
	if (currentTest)
	{
        //std::cout << "current test is  not null" << std::endl;
        currentTest->keyCallBack(window, key, scancode, action, mods);
	}

}

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
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        

        // This works because of c++ polymorphism and because the methods in Test are virtual
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture>("Generate Texture");
        testMenu->RegisterTest<test::TestTriangle>("Generate Triangle");
        testMenu->RegisterTest<test::TestCube>("Generate Cube");

        //glfwSetWindowUserPointer(window, currentTest);
        glfwSetKeyCallback(window, masterKeyCallBack); // for keyboard input

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


            if (currentTest)
            {
                glfwSetWindowUserPointer(window, currentTest);
                currentTest->OnUpdate(delta_time);
                currentTest->OnRender();
                ImGui::Begin("test");
                // to get back to the menu
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }

                currentTest->OnImGuiRender();
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

        // at the end of this scope, IndexBuffer will be deleted (because is Stack allocated)
        // preventing infinit loop of glGetError() 
        // the Buffers can also be heap allocated
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}