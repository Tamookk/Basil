/*
 * This class manages application run-time
 */
#pragma once

#include "ApplicationEvent.h"
#include "Core.h"
#include "Event.h"
#include "Log.h"

namespace Basil
{
	class BASIL_API Application
	{
		public:
			Application();
			virtual ~Application();
			void run();
	};

	// To be defined in the client
	Application* createApplication();
}
