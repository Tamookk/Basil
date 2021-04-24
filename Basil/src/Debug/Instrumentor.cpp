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
		// Close current session before starting new one
		std::lock_guard lock(mutex);
		if (currentSession)
		{
			if (Log::getEngineLogger())	// Logger may not be initialised before profiling starts
				LOG_ERROR("Instrumentor::beginSession('{0}') when session '{1}' already open.", name, currentSession->name);

			// End the current session
			internalEndSession();
		}
		
		// Open file
		outputStream.open(filePath);

		// Check that file opened
		if (outputStream.is_open())
		{
			currentSession = new InstrumentationSession({ name });
			writeHeader();
		}
		else
		{
			if (Log::getEngineLogger())
				LOG_ERROR("Instrumentor could not open results file '{0}'.", filePath);
		}
	}

	// End a session
	void Instrumentor::endSession()
	{
		std::lock_guard lock(mutex);
		internalEndSession();
		writeFooter();
		outputStream.close();
		delete currentSession;
		currentSession = nullptr;
		profileCount = 0;
	}

	// Write the profiling data
	void Instrumentor::writeProfile(const ProfileResult& result)
	{
		// Create string stream
		std::stringstream json;

		// Read in name
		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		// Write data
		if (profileCount++ > 0)
			json << ",";

		json << "{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.end - result.start) << ',';
		json << "\"name\":\"" << name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.threadID << ",";
		json << "\"ts\":" << result.start;
		json << "}";

		// Lock the data and write to file
		std::lock_guard lock(mutex);
		if (currentSession)
		{
			outputStream << json.str();
			outputStream.flush();
		}
	}

	// Get the instrumentor instance
	Instrumentor& Instrumentor::get()
	{
		static Instrumentor instance;
		return instance;
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

	void Instrumentor::internalEndSession()
	{
		if (currentSession)
		{
			writeFooter();
			outputStream.close();
			delete currentSession;
			currentSession = nullptr;
		}
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
		Instrumentor::get().writeProfile({ name, start, end, std::this_thread::get_id() });

		stopped = true;
	}
}