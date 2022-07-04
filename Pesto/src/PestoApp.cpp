#include "Core/Basil.h"
#include "Core/EntryPoint.h"
#include "EditorLayer.h"

namespace Basil
{
	class Pesto : public Application
	{
		public:
			Pesto(const ApplicationSpecification& spec) : Application(spec)
			{
				pushLayer(new EditorLayer());
			}

			~Pesto()
			{

			}
		};

	// Create a new instance of Sandbox
	Application* createApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.name = "Pesto";
		spec.applicationCommandLineArgs = args;

		return new Pesto(spec);
	}
}