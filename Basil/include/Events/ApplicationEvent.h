/*
 * This header defines classes for events relating to the application.
 */
#pragma once

#include "Events/Event.h"

namespace Basil
{
	class WindowCloseEvent : public Event
	{
		public:
			WindowCloseEvent() = default;
			EventType getEventType() const override;
			static EventType getStaticType();
			const char* getName() const override;
			int getCategory() const override;
	};

	class WindowResizeEvent : public Event
	{
		public:
			WindowResizeEvent(unsigned int width, unsigned int height);
			unsigned int getWidth() const;
			unsigned int getHeight() const;
			EventType getEventType() const override;
			static EventType getStaticType();
			const char* getName() const override;
			std::string toString() const override;
			int getCategory() const override;
		private:
			unsigned int width;
			unsigned int height;

	};

	class WindowFocusEvent : public Event
	{
		public:
			WindowFocusEvent() = default;
			EventType getEventType() const override;
			static EventType getStaticType();
			const char* getName() const override;
			int getCategory() const override;
	};

	class WindowLostFocusEvent : public Event
	{
		public:
			WindowLostFocusEvent() = default;
			EventType getEventType() const override;
			static EventType getStaticType();
			const char* getName() const override;
			int getCategory() const override;
	};

	class WindowMovedEvent : public Event
	{
		public:
			WindowMovedEvent() = default;
			EventType getEventType() const override;
			static EventType getStaticType();
			const char* getName() const override;
			int getCategory() const override;
	};

	class AppTickEvent : public Event
	{
		public:
			AppTickEvent() = default;
			EventType getEventType() const override;
			static EventType getStaticType();
			const char* getName() const override;
			int getCategory() const override;
	};

	class AppUpdateEvent : public Event
	{
		AppUpdateEvent() = default;
		EventType getEventType() const override;
		static EventType getStaticType();
		const char* getName() const override;
		int getCategory() const override;
	};

	class AppRenderEvent : public Event
	{
		AppRenderEvent() = default;
		EventType getEventType() const override;
		static EventType getStaticType();
		const char* getName() const override;
		int getCategory() const override;
	};
}
