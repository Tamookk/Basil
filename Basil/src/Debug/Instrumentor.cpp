#include "pch.h"
#include "Debug/Instrumentor.h"

namespace Basil
{
	// -- Instrumentor -- //
	// Constructor
	Instrumentor::Instrumentor()
	{
		currentSession = nullptr;
		profileCount = 0;
	}

	// Begin a session
	void Instrumentor::beginSession(const std::string& name, const std::string& filePath)
	{
		outputStream.open(filePath);
		writeHeader();
		currentSession = new InstrumentationSession{ name };
	}

	// End a session
	void Instrumentor::endSession()
	{
		writeFooter();
		outputStream.close();
		delete currentSession;
		currentSession = nullptr;
		profileCount = 0;
	}

	// Write the profiling data
	void Instrumentor::writeProfile(const ProfileResult& result)
	{
		if (profileCount++ > 0)
			outputStream << ",";

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		outputStream << "{";
		outputStream << "\"cat\":\"function\",";
		outputStream << "\"dur\":" << (result.end - result.start) << ',';
		outputStream << "\"name\":\"" << name << "\",";
		outputStream << "\"ph\":\"X\",";
		outputStream << "\"pid\":0,";
		outputStream << "\"tid\":" << result.threadID << ",";
		outputStream << "\"ts\":" << result.start;
		outputStream << "}";

		outputStream.flush();
	}

	// Write the header
	void Instrumentor::writeHeader()
	{
		outputStream << "{\"otherData\": {},\"traceEvents\":[";
		outputStream.flush();
	}

	// Write the footer
	void Instrumentor::writeFooter()
	{
		outputStream << "]}";
		outputStream.flush();
	}

	// Get the instrumentor instance
	Instrumentor& Instrumentor::get()
	{
		static Instrumentor instance;
		return instance;
	}


	// -- InstrumentationTimer -- //
	// Constructor
	InstrumentationTimer::InstrumentationTimer(const char* name)
	{
		// Initialise variables
		this->name = name;
		stopped = false;
		startTimepoint = std::chrono::high_resolution_clock::now();
	}

	// Destructor
	InstrumentationTimer::~InstrumentationTimer()
	{
		if (!stopped)
			stop();
	}

	// Stop the timer
	void InstrumentationTimer::stop()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::get().writeProfile({ name, start, end, threadID });

		stopped = true;
	}
}