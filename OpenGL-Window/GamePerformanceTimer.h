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
			std::string name;
			double time = 0.0;
			unsigned int ticks = 0;
			double cumulative = 0.0;
			std::chrono::high_resolution_clock::time_point start;
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
		unsigned int Ticks(std::string name)
		{
			_mutex.lock();
			unsigned int ret = 0;
			try
			{
				ret = _timedFuncs.at(name).ticks;
			}
			catch (...)
			{
				ret = -1;
			}
			_mutex.unlock();
			return ret;
		}
		// Returns the nanoseconds the function took on the last call, -1 if not found
		double LastRun(std::string name)
		{
			_mutex.lock();
			double ret = 0.0;
			try
			{
				ret = _timedFuncs.at(name).time;
			}
			catch (...)
			{
				ret = -1.0;
			}
			_mutex.unlock();
			return ret;
		}
		// Returns the total nanoseconds the function ran over all the calls, -1 if not found
		double Cumulative(std::string name)
		{
			_mutex.lock();
			double ret = 0.0;
			try
			{
				ret = _timedFuncs.at(name).cumulative;
			}
			catch (...)
			{
				ret = -1.0;
			}
			_mutex.unlock();
		}
		// Returns the average nanoseconds the function ran over all the calls, -1 if not found
		double Average(std::string name)
		{
			_mutex.lock();
			double ret = 0.0;
			try
			{
				ret = _timedFuncs.at(name).cumulative / _timedFuncs.at(name).ticks;
			}
			catch (...)
			{
				ret = -1.0;
			}
			_mutex.unlock();
			return ret;
		}
		// Returns an unmodifiable container of timed functions
		const std::unordered_map <std::string, _FunctionToTime>& TimedFunctions()
		{
			return _timedFuncs;
		}
		// Start timing
		void Start(std::string name)
		{
			_FunctionToTime t;
			_mutex.lock();
			_timedFuncs.try_emplace(name, t);
			_timedFuncs[name].name = name;
			_timedFuncs[name].start = std::chrono::high_resolution_clock::now();
			_timedFuncs[name].ticks++;
			_mutex.unlock();
		};
		// Stop timing
		void Stop(std::string name)
		{
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
			_mutex.lock();
			try
			{
				_timedFuncs.at(name).time = (double)(duration(timeNow() - _timedFuncs.at(name).start));
				_timedFuncs.at(name).cumulative += _timedFuncs[name].time;
			}
			catch (...)
			{
				// Output to a log or something
				//std::cout << __func__ << ": \"" << name << "\" is not found." << "\n";
			}
			_mutex.unlock();

#undef duration
#undef timeNow
		};
	};


}