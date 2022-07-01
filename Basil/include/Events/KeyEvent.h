/*
 * This header defines key event classes.
 */
#pragma once

#include "Events/Event.h"

namespace Basil
{
	class KeyPressedEvent : public Event
	{
		public:
			KeyPressedEvent(int keycode, bool isRepeat = false);
			int getKeycode() const;
			bool isRepeating() const;
			virtual EventType getEventType() const override;
			static EventType getStaticType();
			virtual const char* getName() const override;
			virtual std::string toString() const override;
			virtual int getCategory() const override;
		private:
			int keycode;
			bool isRepeat;
	};

	class KeyReleasedEvent : public Event
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

	class KeyTypedEvent : public Event
	{
	public:
		KeyTypedEvent(int keycode);
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
