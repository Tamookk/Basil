/*
 * This header defines classes for events relating to the application 
 */
#pragma once

#include <sstream>

#include "Event.h"

namespace Basil
{
	class BASIL_API WindowCloseEvent : public Event
	{
		public:
			WindowCloseEvent();
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual int getCategory() const override;
	};

	class BASIL_API WindowResizeEvent : public Event
	{
		public:
			WindowResizeEvent(unsigned int width, unsigned int height);
			unsigned int getWidth() const;
			unsigned int getHeight() const;
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual std::string toString() const override;
			virtual int getCategory() const override;
		private:
			unsigned int width;
			unsigned int height;

	};

	class BASIL_API WindowFocusEvent : public Event
	{
		public:
			WindowFocusEvent();
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual int getCategory() const override;
	};

	class BASIL_API WindowLostFocusEvent : public Event
	{
		public:
			WindowLostFocusEvent();
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual int getCategory() const override;
	};

	class BASIL_API WindowMovedEvent : public Event
	{
		public:
			WindowMovedEvent();
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual int getCategory() const override;
	};

	class BASIL_API AppTickEvent : public Event
	{
		public:
			AppTickEvent();
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual int getCategory() const override;
	};

	class BASIL_API AppUpdateEvent : public Event
	{
		AppUpdateEvent();
		virtual EventType getEventType() const override;
		static EventType getStaticType();
		virtual const char* getName() const override;
		virtual int getCategory() const override;
	};

	class BASIL_API AppRenderEvent : public Event
	{
		AppRenderEvent();
		virtual EventType getEventType() const override;
		static EventType getStaticType();
		virtual const char* getName() const override;
		virtual int getCategory() const override;
	};
}
