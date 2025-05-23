#include "TestCube.h"
#include "keyboard.h"

#include "imgui/imgui.h"

float FACTOR = 1.f;

namespace test
{

	TestCube::TestCube()
		: m_angleZ(300.0f), m_Proj(glm::ortho(-430.0f, 430.0f, -270.0f, 270.0f, -m_angleZ, m_angleZ)),
		m_viewX(430), m_viewY(220), m_viewZ(0),
		m_scale(1.0f),
		m_translationA(0, 0, 0), m_angleX(45.0f), m_angleY(-45.0f)
	{
		float positions[] = {
			// xyz  Position               // RGBA

			// front 
			-100.0f, -100.0f,   100.0f,    1.0f, 0.0f, 0.0f, 1.0f,  // bottom left 
			100.0f,  -100.0f,   100.0f,    1.0f, 0.0f, 0.0f, 1.0f,  // bottom right
			100.0f,  100.0f, 100.0f,       1.0f, 0.0f, 0.0f, 1.0f,    // top right
			-100.0f, 100.0f, 100.0f,       1.0f, 0.0f, 0.0f, 1.0f,    // top left


			// top
			-100.0f, 100.0f, 100.0f,       0.0f, 1.0f, 0.0f, 1.0f,    // bottom left
			100.0f,  100.0f, 100.0f,       0.0f, 1.0f, 0.0f, 1.0f,    // bottom right
			100.0f,  100.0f, -100.0f,      0.0f, 1.0f, 0.0f, 1.0f,  // top right
			-100.0f, 100.0f, -100.0f,      0.0f, 1.0f, 0.0f, 1.0f,  // top left 

			// right 
			100.0f,  -100.0f,   100.0f,    0.0f, 0.0f, 1.0f, 1.0f,  // bottom left
			100.0f,  -100.0f, -100.0f,     0.0f, 0.0f, 1.0f, 1.0f,  // bottom right
			100.0f,  100.0f, -100.0f,      0.0f, 0.0f, 1.0f, 1.0f,  // top right
			100.0f,  100.0f, 100.0f,       0.0f, 0.0f, 1.0f, 1.0f,    // top left

			// left
			-100.0f, -100.0f,  -100.0f,    1.0f, 0.0f, 1.0f, 1.0f,  // bottom left
			-100.0f, -100.0f,   100.0f,    1.0f, 0.0f, 1.0f, 1.0f,  // bottom right
			-100.0f, 100.0f, 100.0f,       1.0f, 0.0f, 1.0f, 1.0f,  // top right
			-100.0f,  100.0f, -100.0f,     1.0f, 0.0f, 1.0f, 1.0f,    // top left

			// back 
			-100.0f, -100.0f,  -100.0f,    1.0f, 1.0f, 0.0f, 1.0f,  // bottom left 
			100.0f,  -100.0f,  -100.0f,    1.0f, 1.0f, 0.0f, 1.0f,  // bottom right
			100.0f,  100.0f,-100.0f,       1.0f, 1.0f, 0.0f, 1.0f,    // top right
			-100.0f, 100.0f,-100.0f,       1.0f, 1.0f, 0.0f, 1.0f,    // top left

			// bottom
			-100.0f,-100.0f, 100.0f,       0.0f, 1.0f, 1.0f, 1.0f,    // bottom left
			100.0f, -100.0f, 100.0f,       0.0f, 1.0f, 1.0f, 1.0f,    // bottom right
			100.0f, -100.0f, -100.0f,      0.0f, 1.0f, 1.0f, 1.0f,  // top right
			-100.0f,-100.0f, -100.0f,      0.0f, 1.0f, 1.0f, 1.0f,  // top left 
		};

		unsigned int indices[] = { 
			// front 
			0, 1, 2,
			2, 3, 0,

			//top
			4, 5, 6,
			6, 7, 4,

			// right 
			8, 9, 10,
			10, 11, 8,

			// left 
			12, 13, 14,
			14, 15, 12,

			// back
			16, 17, 18, 
			18, 19, 16,

			//bottom
			20, 21, 22, 
			22, 23, 20
		};

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 7 * 4 * 6 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(0);
		layout.Push<float>(4);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6 * 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic_shader.glsl");
		m_Shader->Bind();
		//m_Shader->SetUniform4f("v_Ver")


	}

	TestCube::~TestCube()
	{

	}

	void TestCube::OnUpdate(float deltatime)
	{
		// cube rotation
		if (m_angleX > 180.0f)
		{
			m_angleX = -180.f;
		}
		else {
			m_angleX += 1.0f * deltatime * 15;
		}
		if (m_angleY > 180.0f)
		{
			m_angleY = -180.f;
		}
		else {
			m_angleY += 1.0f * deltatime * 15;
		}
		
		// cube scale
		/*if (m_scale > 1.5f || m_scale < 0.2)
		{
			FACTOR *= -1.f;
		}
		m_scale += FACTOR * 0.25f * deltatime;*/
		

	}

	void TestCube::OnRender()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		GLint depthFuncValue;
		glGetIntegerv(GL_DEPTH_FUNC, &depthFuncValue);
		glClearDepth(depthFuncValue == GL_LESS ? 100.0f : -100.0f);

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer renderer;

		{
			glm::mat4 model = glm::mat4(1.0f);
			m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -m_angleZ, m_angleZ);
			model = glm::rotate(model, glm::radians(m_angleX), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(m_angleY), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, m_translationA);

			glm::vec3 scale = glm::vec3(m_scale, m_scale, m_scale);
			model = glm::scale(model, scale);

			glm::mat4 m_View(glm::translate(glm::mat4(1.0f), glm::vec3(m_viewX, m_viewY, m_viewZ)));

			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestCube::OnImGuiRender()
	{
		ImGui::SliderFloat3("m_translationA", &m_translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat("Rotation X", &m_angleX, -180.0f, 180.0f);
		ImGui::SliderFloat("Rotation Y", &m_angleY, -180.0f, 180.0f);		
		ImGui::SliderFloat("view x", &m_viewX, -500.0f, 500.0f);		
		ImGui::SliderFloat("view y", &m_viewY, -500.0f, 500.0f);			

	}


	void TestCube::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_REPEAT || action == GLFW_PRESS)
		{
			switch (key)
			{
				case GLFW_KEY_D:
				{
					std::cout << "case w" << std::endl;
					m_viewX += 10.0f; // Increment m_viewX			
				}break;
				case GLFW_KEY_A:
				{
					std::cout << "case S" << std::endl;
					m_viewX -= 10.0f; // Increment m_viewX			
				}break;
				case GLFW_KEY_SPACE:
				{
					m_viewY -= 10.0f;
				}break;
				case GLFW_KEY_LEFT_CONTROL:
				{
					m_viewY += 10.0f;
				}break;
				case GLFW_KEY_W:
				{
					m_scale += .1f;
				}break;
				case GLFW_KEY_S:
				{
					m_scale -= .1f;
				}break;
			}
		}		

	}
}