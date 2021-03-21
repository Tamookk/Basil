/*
 * This header defines classes and components for event handling
 */
#pragma once

#include "pch.h"
#include "Core.h"

namespace Basil
{
	// Events are currently processed as they happen

	// Type of event
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Category of event
	enum EventCategory
	{
		None				= 0,
		ApplicationEvent	= 0b00000001,
		InputEvent			= 0b00000010,
		KeyboardEvent		= 0b00000100,
		MouseEvent			= 0b00001000,
		MouseButtonEvent	= 0b00010000
	};

	class BASIL_API Event
	{
		friend class EventDispatcher;
		public:
			Event();
			virtual EventType getEventType() const = 0;
			virtual const char* getName() const = 0;
			virtual std::string toString() const;
			virtual int getCategory() const = 0;
			bool isInCategory(EventCategory category);
		protected:
			bool handled;
	};

	// For dispatching events
	class EventDispatcher
	{
		template <typename T>
		using EventFunction = std::function<bool(T&)>;
		public:
			EventDispatcher(Event& e);
			template <typename T>
			bool dispatch(EventFunction<T> eventFunction);
		private:
			Event& event;
	};

	// Define << operator for events
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}
