#include "Core/Basil.h"
#include "Core/EntryPoint.h"
#include "EditorLayer.h"

namespace Basil
{
	class Pesto : public Application
	{
		public:
			Pesto(ApplicationCommandLineArgs args) : Application("Pesto", args)
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
		return new Pesto(args);
	}
}