/*
 * This header defines key event classes 
 */
#pragma once

#include "Event.h"

namespace Basil
{
	class BASIL_API KeyPressedEvent : public Event
	{
		public:
			KeyPressedEvent(int keycode, int repeatCount);
			int getKeycode() const;
			int getRepeatCount() const;
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual std::string toString() const override;
			virtual int getCategory() const override;
		private:
			int keycode;
			int repeatCount;
	};

	class BASIL_API KeyReleasedEvent : public Event
	{
		public:
			KeyReleasedEvent(int keycode);
			int getKeycode() const;
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual std::string toString() const override;
			virtual int getCategory() const override;
		private:
			int keycode;
	};
}