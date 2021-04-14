/*
 * This header defines mouse event classes.
 */
#pragma once

#include "Event.h"

namespace Basil
{
	class BASIL_API MouseButtonPressedEvent : public Event
	{
		public:
			MouseButtonPressedEvent(int button);
			int getMouseButton() const;
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual std::string toString() const override;
			virtual int getCategory() const override;
		private:
			int button;
	};

	class BASIL_API MouseButtonReleasedEvent : public Event
	{
		public:
			MouseButtonReleasedEvent(int button);
			int getMouseButton() const;
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual std::string toString() const override;
			virtual int getCategory() const override;
		private:
			int button;
	};

	class BASIL_API MouseMovedEvent : public Event
	{
		public:
			MouseMovedEvent(float x, float y);
			float getX() const;
			float getY() const;
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual std::string toString() const override;
			virtual int getCategory() const override;
		private:
			float mouseX;
			float mouseY;
	};

	class BASIL_API MouseScrolledEvent : public Event
	{
		public:
			MouseScrolledEvent(float xOffset, float yOffset);
			float getXOffset() const;
			float getYOffset() const;
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual std::string toString() const override;
			virtual int getCategory() const override;
		private:
			float xOffset;
			float yOffset;
	};
}