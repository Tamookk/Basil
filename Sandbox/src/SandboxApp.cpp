#include "Core/Basil.h"
#include "Core/EntryPoint.h"
#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Basil::Application
{
	public:
		Sandbox()
		{
			//pushLayer(new ExampleLayer());
			pushLayer(new Sandbox2D());
		}

		~Sandbox()
		{

		}
};

// Create a new instance of Sandbox
Basil::Application* Basil::createApplication(Basil::ApplicationCommandLineArgs args)
{
	return new Sandbox();
}