#include "Core/Basil.h"
#include "Core/EntryPoint.h"
#include "EditorLayer.h"

class Pesto : public Basil::Application
{
public:
	Pesto() : Application("Pesto")
	{
		//pushLayer(new ExampleLayer());
		pushLayer(new EditorLayer());
	}

	~Pesto()
	{

	}
};

// Create a new instance of Sandbox
Basil::Application* Basil::createApplication()
{
	return new Pesto();
}