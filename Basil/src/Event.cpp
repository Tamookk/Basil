#include "Event.h"

namespace Basil
{
	// -- Event -- //
	// Constructor
	Event::Event()
	{
		handled = false;
	}

	// Return the string representation of the event
	std::string Event::toString() const
	{
		return getName();
	}

	// Returns true if the category if the event is in the category specified by the parameter
	bool Event::isInCategory(EventCategory category)
	{
		return getCategory() & category;
	}


	// -- Event Dispatcher -- //
	// Constructor
	EventDispatcher::EventDispatcher(Event& e) : event(e) {}

	// Dispatch an event
	template <typename T>
	bool EventDispatcher::dispatch(EventFunction<T> function)
	{
		if (event.getEventType == T::getStaticType())
		{
			event.handled = function(*(T*)&event);
			return true;
		}
		return false;
	}
}