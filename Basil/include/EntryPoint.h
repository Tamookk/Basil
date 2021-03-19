/*
 * This class manages the entry point for an application created
 * using Basil.
 */

#pragma once

#include "Application.h"
#include "Log.h"

#ifdef PLATFORM_WINDOWS
extern Basil::Application* Basil::createApplication();

int main(int argc, char* argv[])
{
	// Initialise loggers
	Basil::Log::init();
	LOG_WARN("aaaaaaaAAAAAAAAAAA");
	LOG_FATAL("x = {0}", 5);

	auto app = Basil::createApplication();
	app->run();
	delete app;
	return 0;
}
#else
	#error Basil only supports Windows
#endif
