/*
 * This header file declares a generic Window class.
 */
#pragma once

#include "pch.h"
#include "Core/Core.h"
#include "Events/Event.h"

namespace Basil
{
	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Basil Game Engine", unsigned int width = 1280, unsigned int height = 720)
		{
			this->title = title;
			this->width = width;
			this->height = height;
		}
	};

	class Window
	{
		public:
			using EventCallbackFunction = std::function<void(Event&)>;
			virtual void onUpdate() = 0;
			virtual unsigned int getWidth() const = 0;
			virtual unsigned int getHeight() const = 0;
			virtual void setEventCallback(const EventCallbackFunction& callback) = 0;
			virtual void setVsync(bool enabled) = 0;
			virtual bool isVsync() const = 0;
			virtual void* getNativeWindow() const = 0;

			static Unique<Window> create(const WindowProps& props = WindowProps());
	};
}