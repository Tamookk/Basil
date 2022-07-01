#include "pch.h"
#include "Events/KeyEvent.h"

namespace Basil
{
	// -- Key Pressed Event -- //
	KeyPressedEvent::KeyPressedEvent(int keycode, bool isRepeat)
	{
		this->keycode = keycode;
		this->isRepeat = isRepeat;
	}

	int KeyPressedEvent::getKeycode() const { return keycode; }
	bool KeyPressedEvent::isRepeating() const { return isRepeat; }
	EventType KeyPressedEvent::getEventType() const { return KeyPressedEvent::getStaticType(); }
	EventType KeyPressedEvent::getStaticType() { return EventType::KeyPressed; }
	const char* KeyPressedEvent::getName() const { return "KeyPressed"; }

	std::string KeyPressedEvent::toString() const
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << keycode << " (repeat = " << isRepeat << " repeats)";
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


	// -- Key Typed Event -- //
	KeyTypedEvent::KeyTypedEvent(int keycode)
	{
		this->keycode = keycode;
	}

	int KeyTypedEvent::getKeycode() const { return keycode; }
	EventType KeyTypedEvent::getEventType() const { return KeyTypedEvent::getStaticType(); }
	EventType KeyTypedEvent::getStaticType() { return EventType::KeyTyped; }
	const char* KeyTypedEvent::getName() const { return "KeyTyped"; }

	std::string KeyTypedEvent::toString() const
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << getKeycode();
		return ss.str();
	}

	int KeyTypedEvent::getCategory() const { return EventCategory::KeyboardEvent | EventCategory::InputEvent; }
}