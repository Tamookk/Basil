/*
 * This cpp file is used to test the Basil Game Engine as it is being developed. 
 */

#include "Basil.h"

class ExampleLayer : public Basil::Layer
{
	public:
		ExampleLayer() : Layer("Example") {}
		void onUpdate() override
		{
			LOG_CLIENT_INFO("ExampleLayer::Update");
		}

		void onEvent(Basil::Event& e) override
		{
			LOG_CLIENT_TRACE("{0}", e);
		}
};

class Sandbox : public Basil::Application
{
	public:
		Sandbox()
		{
			pushLayer(new ExampleLayer());
			pushOverlay(new Basil::ImGuiLayer());
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