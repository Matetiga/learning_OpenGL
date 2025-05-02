#pragma once


namespace test {
	class Test
	{
	public :
		Test(){}
		virtual ~Test(){}

		// Virtual Function's bodies can be overriden in derived classes 
		virtual void OnUpdate(float deltatime){}
		virtual void OnRender(){}
		virtual void OnImGuiRender(){}
	};
}