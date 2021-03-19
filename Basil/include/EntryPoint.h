/*
 * This header declares the entry point for an application created
 * using Basil.
 */

#pragma once

#include "Application.h"

#ifdef PLATFORM_WINDOWS
extern Basil::Application* Basil::createApplication();

int main(int argc, char* argv[])
{
	auto app = Basil::createApplication();
	app->run();
	delete app;
	return 0;
}
#else
	#error Basil only supports Windows
#endif
