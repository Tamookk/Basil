#include "pch.h"
#include "Events/MouseEvent.h"

namespace Basil
{
	// -- Mouse Button Pressed Event -- //
	MouseButtonPressedEvent::MouseButtonPressedEvent(int button)
	{
		this->button = button;
	}

	int MouseButtonPressedEvent::getMouseButton() const { return button; }
	EventType MouseButtonPressedEvent::getEventType() const { return MouseButtonPressedEvent::getStaticType(); }
	EventType MouseButtonPressedEvent::getStaticType() { return EventType::MouseButtonPressed; }
	const char* MouseButtonPressedEvent::getName() const { return "MouseButtonPressed"; }

	std::string MouseButtonPressedEvent::toString() const
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << button;
		return ss.str();
	}

	int MouseButtonPressedEvent::getCategory() const { return EventCategory::MouseEvent | EventCategory::InputEvent; }


	// -- Mouse Button Released Event -- //
	MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button)
	{
		this->button = button;
	}

	int MouseButtonReleasedEvent::getMouseButton() const { return button; }
	EventType MouseButtonReleasedEvent::getEventType() const { return MouseButtonReleasedEvent::getStaticType(); }
	EventType MouseButtonReleasedEvent::getStaticType() { return EventType::MouseButtonReleased; }
	const char* MouseButtonReleasedEvent::getName() const { return "MouseButtonReleased"; }

	std::string MouseButtonReleasedEvent::toString() const
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << button;
		return ss.str();
	}

	int MouseButtonReleasedEvent::getCategory() const { return EventCategory::MouseEvent | EventCategory::InputEvent; }


	// -- Mouse Moved Event -- //
	MouseMovedEvent::MouseMovedEvent(float x, float y)
	{
		mouseX = x;
		mouseY = y;
	}

	float MouseMovedEvent::getX() const { return mouseX; }
	float MouseMovedEvent::getY() const { return mouseY; }
	EventType MouseMovedEvent::getEventType() const { return MouseMovedEvent::getStaticType(); };
	EventType MouseMovedEvent::getStaticType() { return EventType::MouseMoved; }
	const char* MouseMovedEvent::getName() const { return "MouseMoved"; }

	std::string MouseMovedEvent::toString() const
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
		return ss.str();
	}

	int MouseMovedEvent::getCategory() const { return EventCategory::MouseEvent | EventCategory::InputEvent; }


	// -- Mouse Scrolled Event -- //
	MouseScrolledEvent::MouseScrolledEvent(float xOffset, float yOffset)
	{
		this->xOffset = xOffset;
		this->yOffset = yOffset;
	}

	float MouseScrolledEvent::getXOffset() const { return xOffset; }
	float MouseScrolledEvent::getYOffset() const { return yOffset; }
	EventType MouseScrolledEvent::getEventType() const { return MouseScrolledEvent::getStaticType(); }
	EventType MouseScrolledEvent::getStaticType() { return EventType::MouseScrolled; }
	const char* MouseScrolledEvent::getName() const { return "MouseScrolled"; }

	std::string MouseScrolledEvent::toString() const
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
		return ss.str();
	}

	int MouseScrolledEvent::getCategory() const { return EventCategory::MouseEvent | EventCategory::InputEvent; }
}