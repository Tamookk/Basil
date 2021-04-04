/*
 * This header file contains an extension of the Window class that is platform-specific to Windows
 */
#pragma once

#include "ApplicationEvent.h"
#include "GraphicsContext.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "OpenGLContext.h"
#include "Window.h"

namespace Basil
{
	class WindowsWindow : public Window
	{
		public:
			WindowsWindow(const WindowProps& props);
			virtual ~WindowsWindow();
			void onUpdate() override;
			unsigned int getWidth() const override;
			unsigned int getHeight() const override;
			void setEventCallback(const EventCallbackFunction& callback) override;
			void setVsync(bool enabled) override;
			bool isVsync() const override;
			void* getNativeWindow() const override;
		private:
			struct WindowData
			{
				std::string title;
				unsigned int width;
				unsigned int height;
				bool vsync;
				EventCallbackFunction eventCallbackFunction;
			};
			virtual void init(const WindowProps& props);
			virtual void shutdown();
			GLFWwindow* window;
			GraphicsContext* context;
			WindowData data;
	};
}
