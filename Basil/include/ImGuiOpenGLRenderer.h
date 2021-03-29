/*
 * This header file and its cpp implementation is for testing purposes. It has been copied from
 * https://github.com/TheCherno/Hazel/blob/c65520c96289ab62ecd37995108063ce4019d4e9/Hazel/src/Platform/OpenGL/ImGuiOpenGLRenderer.h
 * It is a renderer for modern OpenGL.
 */
#pragma once

#include "imgui.h"

IMGUI_IMPL_API bool		ImGui_ImplOpenGL3_Init(const char* glsl_version = NULL);
IMGUI_IMPL_API void		ImGui_ImplOpenGL3_Shutdown();
IMGUI_IMPL_API void		ImGui_ImplOpenGL3_NewFrame();
IMGUI_IMPL_API void		ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

// Called by init/newframe/shutdown
IMGUI_IMPL_API bool		ImGui_ImplOpenGL3_CreateFontsTexture();
IMGUI_IMPL_API void		ImGui_ImplOpenGL3_DestroyFontsTexture();
IMGUI_IMPL_API bool		ImGui_ImplOpenGL3_CreateDeviceObjects();
IMGUI_IMPL_API void		ImGui_ImplOpenGL3_DestroyDeviceObjects();