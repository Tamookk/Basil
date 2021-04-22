/*
 * This header defines classes and components for event handling.
 */
#pragma once

#include "pch.h"
#include "Core/Core.h"

namespace Basil
{
	// Events are currently processed as they happen

	// Type of event
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
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

	class Event
	{
		friend class EventDispatcher;
		public:
			Event();
			virtual EventType getEventType() const = 0;
			virtual const char* getName() const = 0;
			virtual std::string toString() const;
			virtual int getCategory() const = 0;
			bool isInCategory(EventCategory category);
			bool handled;
	};

	// For dispatching events
	class EventDispatcher
	{
		public:
			EventDispatcher(Event& e);
			template <typename T, typename F>
			bool dispatch(const F& eventFunction)
			{
				if (event.getEventType() == T::getStaticType())
				{
					event.handled = eventFunction(static_cast<T&>(event));
					return true;
				}
				return false;
			}
		private:
			Event& event;
	};

	// Define << operator for events
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}
