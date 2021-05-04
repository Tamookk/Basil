#include "Core/Basil.h"
#include "Core/EntryPoint.h"
#include "EditorLayer.h"

namespace Basil
{
	class Pesto : public Application
	{
		public:
			Pesto() : Application("Pesto")
			{
				pushLayer(new EditorLayer());
			}

			~Pesto()
			{

			}
		};

	// Create a new instance of Sandbox
	Application* createApplication()
	{
		return new Pesto();
	}
}