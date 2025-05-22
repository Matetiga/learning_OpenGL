#pragma once

#include "test.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {

	class TestCube : public Test
	{
	public:
		TestCube();
		~TestCube();

		void OnUpdate(float deltatime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) override;

	private:
		// smart Pointers for this classes, otherwise they will be initialized using the default constructor
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj;
		glm::vec3 m_translationA;
		float m_angleX, m_angleY, m_angleZ, m_viewX, m_viewY, m_viewZ, m_scale;
	};
}