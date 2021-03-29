#include "pch.h"
#include "KeyEvent.h"

namespace Basil
{
	// -- Key Pressed Event -- //
	KeyPressedEvent::KeyPressedEvent(int keycode, int repeatCount)
	{
		this->keycode = keycode;
		this->repeatCount = repeatCount;
	}

	int KeyPressedEvent::getKeycode() const { return keycode; }
	int KeyPressedEvent::getRepeatCount() const { return repeatCount; }
	EventType KeyPressedEvent::getEventType() const { return KeyPressedEvent::getStaticType(); }
	EventType KeyPressedEvent::getStaticType() { return EventType::KeyPressed; }
	const char* KeyPressedEvent::getName() const { return "KeyPressed"; }

	std::string KeyPressedEvent::toString() const
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << keycode << " (" << repeatCount << " repeats)";
		return ss.str();
	}

	int KeyPressedEvent::getCategory() const { return EventCategory::KeyboardEvent | EventCategory::InputEvent; }

	
	// -- Key Released Event -- //
	KeyReleasedEvent::KeyReleasedEvent(int keycode)
	{
		this->keycode = keycode;
	}

	int KeyReleasedEvent::getKeycode() const { return keycode; }
	EventType KeyReleasedEvent::getEventType() const { return KeyReleasedEvent::getStaticType(); }
	EventType KeyReleasedEvent::getStaticType() { return EventType::KeyReleased; }
	const char* KeyReleasedEvent::getName() const { return "KeyReleased"; }

	std::string KeyReleasedEvent::toString() const
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << getKeycode();
		return ss.str();
	}

	int KeyReleasedEvent::getCategory() const { return EventCategory::KeyboardEvent | EventCategory::InputEvent; }
}