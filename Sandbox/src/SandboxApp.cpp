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
			if (Basil::Input::isKeyPressed(BASIL_KEY_TAB))
				LOG_CLIENT_INFO("Tab key is pressed.");
		}

		void onEvent(Basil::Event& e) override
		{
			if (e.getEventType() == Basil::EventType::KeyPressed)
			{
				Basil::KeyPressedEvent& eeevent = (Basil::KeyPressedEvent&)e;
				if (eeevent.getKeycode() == BASIL_KEY_0)
					LOG_CLIENT_TRACE("Tab key is pressed!");
				LOG_CLIENT_TRACE("{0}", (char)eeevent.getKeycode());
			}

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