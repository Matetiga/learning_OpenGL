#pragma once

#include <vector>
#include <functional>
#include <string>
#include <iostream>

namespace test {
	class Test
	{
	public :
		Test(){}
		virtual ~Test(){}

		// Virtual Function's bodies can be overridden in derived classes 
		virtual void OnUpdate(float deltatime){}
		virtual void OnRender(){}
		virtual void OnImGuiRender(){}
	};


	class TestMenu : public Test
	{
	private:
		// Pointer to a reference
		// This allows m_CurrentTest to alias an existing Test* pointer, so modifying m_CurrentTest changes the pointer it refers to
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;

	public:
		TestMenu(Test*& currentTestPointer);

		// because the methods on the parent class are not 'pure' virtual (with =0 instead of {})
		// It let us decide which ones to implement (not obligatory)
		void OnImGuiRender() override;


		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering Test " << name << std::endl;
			// lambda function creates an instance of the Test
			// lambda 
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	};
}