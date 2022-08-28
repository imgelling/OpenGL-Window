#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <shared_mutex>

namespace game
{
	class PerformanceTimer
	{
	private:
		// Holds relavent data per function timed
		struct _FunctionToTime
		{
			std::string _name;
			double _time = 0.0;
			uint32_t _ticks = 0;
			double _cumulative = 0.0;
			std::chrono::high_resolution_clock::time_point _start;
		};
		// Think I need this if using threads
		std::shared_mutex _mutex;
		// Holds a FunctionToTime keyed to a string specified by user
		std::unordered_map <std::string, _FunctionToTime> _timedFuncs;
	public:
		~PerformanceTimer()
		{
			_timedFuncs.clear();
		}
		// Returns number of times the function was timed, -1 if not found
		uint32_t Ticks(const std::string name);

		// Returns the nanoseconds the function took on the last call, -1 if not found
		double LastRun(const std::string name);

		// Returns the total nanoseconds the function ran over all the calls, -1 if not found
		double Cumulative(const std::string name);

		// Returns the average nanoseconds the function ran over all the calls, -1 if not found
		double Average(const std::string name);

		// Returns an unmodifiable container of timed functions
		const std::unordered_map <std::string, _FunctionToTime>& TimedFunctions()
		{
			return _timedFuncs;
		}

		// Start timing
		void Start(const std::string name);

		// Stop timing
		void Stop(const std::string name);

	};


}