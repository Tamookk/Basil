/*
 * This cpp file is used to test the Basil Game Engine as it is being developed. 
 */

#include <Basil.h>

class Sandbox : public Basil::Application
{
	public:
		Sandbox()
		{

		}

		~Sandbox()
		{

		}
};

// Create a new instance of Sandbox
Basil::Application* Basil::createApplication()
{
	return new Sandbox();
}