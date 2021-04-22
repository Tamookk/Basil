#include "pch.h"
#include "Events/ApplicationEvent.h"

namespace Basil
{
	// -- Window Close Event -- //
	EventType WindowCloseEvent::getEventType() const { return getStaticType(); }
	EventType WindowCloseEvent::getStaticType() { return EventType::WindowClose; }
	const char* WindowCloseEvent::getName() const { return "WindowClose"; }
	int WindowCloseEvent::getCategory() const { return EventCategory::ApplicationEvent; }


	// -- Window Resize Event -- //
	WindowResizeEvent::WindowResizeEvent(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;
	}

	unsigned int WindowResizeEvent::getWidth() const { return width; }
	unsigned int WindowResizeEvent::getHeight() const { return height; }
	EventType WindowResizeEvent::getEventType() const { return getStaticType(); }
	EventType WindowResizeEvent::getStaticType() { return EventType::WindowResize; }
	const char* WindowResizeEvent::getName() const { return "WindowResize"; }
	
	std::string WindowResizeEvent::toString() const
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << width << ", " << height;
		return ss.str();
	}
	
	int WindowResizeEvent::getCategory() const { return EventCategory::ApplicationEvent; }


	// -- Window Focus Event -- //
	EventType WindowFocusEvent::getEventType() const { return getStaticType(); }
	EventType WindowFocusEvent::getStaticType() { return EventType::WindowFocus; }
	const char* WindowFocusEvent::getName() const { return "WindowFocus"; }
	int WindowFocusEvent::getCategory() const { return EventCategory::ApplicationEvent; }


	// -- Window Lost Focus Event -- //
	EventType WindowLostFocusEvent::getEventType() const { return getStaticType(); }
	EventType WindowLostFocusEvent::getStaticType() { return EventType::WindowLostFocus; }
	const char* WindowLostFocusEvent::getName() const { return "WindowLostFocus"; }
	int WindowLostFocusEvent::getCategory() const { return EventCategory::ApplicationEvent; }


	// -- Window Moved Event -- //
	EventType WindowMovedEvent::getEventType() const { return getStaticType(); }
	EventType WindowMovedEvent::getStaticType() { return EventType::WindowMoved; }
	const char* WindowMovedEvent::getName() const { return "WindowMoved"; }
	int WindowMovedEvent::getCategory() const { return EventCategory::ApplicationEvent; }


	// -- App Tick Event -- //
	EventType AppTickEvent::getEventType() const { return getStaticType(); }
	EventType AppTickEvent::getStaticType() { return EventType::AppTick; }
	const char* AppTickEvent::getName() const { return "AppTick"; }
	int AppTickEvent::getCategory() const { return EventCategory::ApplicationEvent; }


	// -- App Update Event -- //
	EventType AppUpdateEvent::getEventType() const { return getStaticType(); }
	EventType AppUpdateEvent::getStaticType() { return EventType::AppUpdate; }
	const char* AppUpdateEvent::getName() const { return "AppUpdate"; }
	int AppUpdateEvent::getCategory() const { return EventCategory::ApplicationEvent; }


	// -- App Render Event -- //
	EventType AppRenderEvent::getEventType() const { return getStaticType(); }
	EventType AppRenderEvent::getStaticType() { return EventType::AppRender; }
	const char* AppRenderEvent::getName() const { return "AppRender"; }
	int AppRenderEvent::getCategory() const { return EventCategory::ApplicationEvent; }
}