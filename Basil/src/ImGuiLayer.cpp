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
		io.DeltaTime = time > 0.0 ? (timeNow - time) : (1.0f/60.0f);
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

	}
}