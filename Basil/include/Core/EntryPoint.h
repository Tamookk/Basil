/*
 * This class manages the entry point for an application created
 * using Basil.
 */

#pragma once

#include "Core/Application.h"
#include "Core/Log.h"

#ifdef PLATFORM_WINDOWS
extern Basil::Application* Basil::createApplication(ApplicationCommandLineArgs args);

int main(int argc, char* argv[])
{
	// Initialise loggers
	Basil::Log::init();

	PROFILE_BEGIN_SESSION("Startup", "Profile-Startup.json");
	auto app = Basil::createApplication({ argc, argv });
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");
	app->run();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Shutdown", "Profile-Shutdown.json");
	delete app;
	PROFILE_END_SESSION();

	return 0;
}
#else
	#error Basil only supports Windows
#endif
