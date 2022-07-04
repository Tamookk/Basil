#include "Core/Basil.h"
#include "Core/EntryPoint.h"
#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Basil::Application
{
	public:
		Sandbox(const Basil::ApplicationSpecification& specification) : Basil::Application(specification)
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
	// Create application specification
	ApplicationSpecification spec;
	spec.name = "Sandbox";
	spec.workingDirectory = "../Pesto";
	spec.applicationCommandLineArgs = args;

	return new Sandbox(spec);
}