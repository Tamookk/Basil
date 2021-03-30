#include "pch.h"
#include "ImGuiLayer.h"

namespace Basil
{
	// Constructor
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
		time = 0.0f;
	}

	// Destructor
	ImGuiLayer::~ImGuiLayer()
	{

	}

	// On attach to layer stack
	void ImGuiLayer::onAttach()
	{
		// Create the context
		ImGui::CreateContext();

		// Use dark colours
		ImGui::StyleColorsDark();

		// Set some flags
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// Map keys - TEMPORARY
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		// Initialise ImGui
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	// On detach from layer stack
	void ImGuiLayer::onDetach()
	{

	}

	// On update
	void ImGuiLayer::onUpdate()
	{
		// Set display size
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

		// Set delta time
		float timeNow = (float)glfwGetTime();
		io.DeltaTime = time > 0.0 ? (timeNow - time) : (1.0f / 60.0f);
		time = timeNow;

		// New frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Show demo window
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		// Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	// On event
	void ImGuiLayer::onEvent(Event& e)
	{
		// Create event dispatcher and dispatch it
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT(ImGuiLayer::onKeyPressedEvent));
		dispatcher.dispatch<KeyReleasedEvent>(BIND_EVENT(ImGuiLayer::onKeyReleasedEvent));
		dispatcher.dispatch<KeyTypedEvent>(BIND_EVENT(ImGuiLayer::onKeyTypedEvent));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT(ImGuiLayer::onMouseButtonPressedEvent));
		dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_EVENT(ImGuiLayer::onMouseButtonReleasedEvent));
		dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT(ImGuiLayer::onMouseMovedEvent));
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT(ImGuiLayer::onMouseScolledEvent));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT(ImGuiLayer::onWindowResizeEvent));
	}

	// On key pressed event
	bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& e)
	{
		// Set IO's keysdown and modifier keys variables
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeycode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		// Return false - don't want to consume every single mouse pressed event here
		return false;
	}

	// On key released event
	bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& e)
	{
		// Set IO's keysdown and modifier keys variables
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.getKeycode()] = true;
		return false;
	}

	// On key typed event (for typing, of course)
	bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.getKeycode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	// On mouse button pressed event
	bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		// Get IO and set its mousedown variable for the button pressed to true
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getMouseButton()] = true;
		return false;
	}

	// On mouse button released event
	bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		// Get IO and set its mousedown variable for the button released to false
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.getMouseButton()] = false;
		return false;
	}

	// On mouse button moved event
	bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& e)
	{
		// Set IO's mousepos variable to the mouse position
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.getX(), e.getY());
		return false;
	}

	// On mouse scrolled event
	bool ImGuiLayer::onMouseScolledEvent(MouseScrolledEvent& e)
	{
		// Change IO's mousewheel variables by amount mouse wheel is scrolled
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.getYOffset();
		io.MouseWheelH += e.getXOffset();
		return false;
	}

	// On window resize event
	bool ImGuiLayer::onWindowResizeEvent(WindowResizeEvent& e)
	{
		// Change IO's displaysize and framebufferscale variables
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.getWidth(), e.getHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		return false;
	}
}