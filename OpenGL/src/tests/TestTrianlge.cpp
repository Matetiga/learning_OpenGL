#include "TestTriangle.h"

#include "imgui/imgui.h"

namespace test
{

	TestTriangle::TestTriangle()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_translationA(200, 200, 0)
	{
		float positions[] = {
			0.0f, 100.0f, // top middle 
			-50.0f, 0.0f, // bottom left 
			50.0f, 0.0f   // bottom right
		};

		unsigned int indices[] = { 0, 1, 2 };

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 2 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic_shader.glsl");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 1.0, 0.0, 0.0, 1.0);

		
	}

	TestTriangle::~TestTriangle()
	{
	}

	void TestTriangle::OnUpdate(float deltatime)
	{
	}

	void TestTriangle::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

	}

	void TestTriangle::OnImGuiRender()
	{
		ImGui::SliderFloat3("m_translationA", &m_translationA.x, 0.0f, 960.0f);
	}
}