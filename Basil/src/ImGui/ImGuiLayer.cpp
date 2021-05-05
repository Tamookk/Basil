#include "pch.h"
#include "Core/Application.h"
#include "ImGui/ImGuiLayer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Basil
{
	// Constructor
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
		blockEvents = true;
		time = 0.0f;
	}

	// On attach to layer stack
	void ImGuiLayer::onAttach()
	{
		PROFILE_FUNCTION();

		// Set up the context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		
		// Use dark colours
		ImGui::StyleColorsDark();

		// When viewports are enabled we make platform windows look identical to regular ones
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Get window
		Application& app = Application::get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

		// Set up platform/render bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	// On detach from layer stack
	void ImGuiLayer::onDetach()
	{
		PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	// On event
	void ImGuiLayer::onEvent(Event& e)
	{
		if (blockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.handled |= e.isInCategory(MouseEvent) & io.WantCaptureMouse;
			e.handled |= e.isInCategory(KeyboardEvent) & io.WantCaptureKeyboard;
		}
	}

	// On ImGui render
	void ImGuiLayer::onImGuiRender() {}

	// On begin
	void ImGuiLayer::begin()
	{
		PROFILE_FUNCTION();

		// New frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	// On end
	void ImGuiLayer::end()
	{
		PROFILE_FUNCTION();

		// Set display size
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// For multi-viewport support
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}
	}

	// Set whether events are blocked
	void ImGuiLayer::setBlockEvents(bool block)
	{
		blockEvents = block;
	}
}