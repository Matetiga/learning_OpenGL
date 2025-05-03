#include "TestTexture.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
	TestTexture::TestTexture()
		:m_translationA(200, 200, 0), m_translationB(500, 200, 0)
	{
		float position[] = {
			// the last two values of each row are the texture coordinate (values of 0 to 1.0) 
			// so each corner of the image is directly bound to each corner of the rect
			-50.0f, -50.0f, 0.0f, 0.0f, // bottom left   --- 0 
			50.0f, -50.0f, 1.0f, 0.0f, // bottom right   --- 1
			50.0f, 50.0f, 1.0f, 1.0f,// top right        --- 2
		   -50.0f, 50.0f, 0.0f, 1.0f, // top left       --- 3
		};

		// Index Buffer -> to avoid re rendering the same vertex twice
		unsigned int indices[]{ // ANY INDEX BUFFER must be UNSIGNED
			0, 1, 2,
			2, 3, 0
		};


		// the four first values give the position of the window itself (to put the edges into coordinates) 
		// bottom left (0, 0)
		// top right (960, 540)
		m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(position, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		// for the texture coordinates
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.3, 0.5, 1, 0.8);

		m_Texture = std::make_unique<Texture>("res/textures/nerd.png");
		m_Shader->SetUniform1i("u_Texture", 0); // Texture is bound to slot 0

	}

	TestTexture::~TestTexture()
	{
	}

	void TestTexture::OnUpdate(float deltatime)
	{
	}

	void TestTexture::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// Uniforms must be bound to the right shader
		Renderer renderer;

		m_Texture->Bind(0); // this must match with the texture slot

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
			glm::mat4 mvp = m_Proj * m_View* model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
			glm::mat4 mvp = m_Proj * m_View* model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}
	void TestTexture::OnImGuiRender()
	{
		ImGui::SliderFloat3("m_translationA", &m_translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("m_translationB", &m_translationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}