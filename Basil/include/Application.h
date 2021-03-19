/*
 * This class manages application run-time
 */
#pragma once

#include "Core.h"

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
