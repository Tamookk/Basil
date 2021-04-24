/*
 * This cpp file is used to test the Basil Game Engine as it is being developed. 
 */

#include "Core/Basil.h"
#include "Core/EntryPoint.h"
#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Basil::Application
{
	public:
		Sandbox()
		{
			pushLayer(new ExampleLayer());
			//pushLayer(new Sandbox2D());
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