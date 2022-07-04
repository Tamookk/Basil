/*
 * This class manages application run-time.
 */
#pragma once

#include "Core/Core.h"
#include "Core/Input.h"
#include "Core/LayerStack.h"
#include "Core/Timestep.h"
#include "Core/Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"

int main(int argc, char** argv);

namespace Basil
{
	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index)
		{
			ASSERT(index < count, "");
			return args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string name = "Basil Application";
		std::string workingDirectory;
		ApplicationCommandLineArgs applicationCommandLineArgs = ApplicationCommandLineArgs();
	};

	class Application
	{
		public:
			Application(const ApplicationSpecification& specification);
			virtual ~Application();
			void close();
			ImGuiLayer* getImGuiLayer();
			void onEvent(Event& e);
			void pushLayer(Layer* layer);
			void pushOverlay(Layer* layer);
			static Application& get();
			const ApplicationSpecification& getSpecification() const;
			ApplicationCommandLineArgs getCommandLineArgs() const;
			Window& getWindow();
		private:
			void run();
			bool onWindowClose(WindowCloseEvent& e);
			bool onWindowResize(WindowResizeEvent& e);
			
			ApplicationCommandLineArgs commandLineArgs;
			ApplicationSpecification specification;
			Unique<Window> window;
			ImGuiLayer* imGuiLayer;
			bool running;
			bool minimised;
			LayerStack layerStack;
			static Application* instance;
			float lastFrameTime;
			friend int ::main(int argc, char** argv);
	};

	// To be defined in the client
	Application* createApplication(ApplicationCommandLineArgs args);
}
