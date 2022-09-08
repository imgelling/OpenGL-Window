#include "GamePerformanceTimer.h"

namespace game
{
	struct _FunctionToTime;
	uint32_t PerformanceTimer::Ticks(const std::string name)
	{
		_mutex.lock();
		unsigned int ret = 0;
		try
		{
			ret = _timedFuncs.at(name)._ticks;
		}
		catch (...)
		{
			ret = -1;
		}
		_mutex.unlock();
		return ret;
	}
	double_t PerformanceTimer::LastRun(const std::string name)
	{
		_mutex.lock();
		double_t ret = 0.0;
		try
		{
			ret = _timedFuncs.at(name)._time;
		}
		catch (...)
		{
			ret = -1.0;
		}
		_mutex.unlock();
		return ret;
	}
	double_t PerformanceTimer::Cumulative(const std::string name)
	{
		_mutex.lock();
		double_t ret = 0.0;
		try
		{
			ret = _timedFuncs.at(name)._cumulative;
		}
		catch (...)
		{
			ret = -1.0;
		}
		_mutex.unlock();
		return ret;
	}
	double_t PerformanceTimer::Average(const std::string name)
	{
		_mutex.lock();
		double_t ret = 0.0;
		try
		{
			ret = _timedFuncs.at(name)._cumulative / _timedFuncs.at(name)._ticks;
		}
		catch (...)
		{
			ret = -1.0;
		}
		_mutex.unlock();
		return ret;
	}
	void PerformanceTimer::Start(const std::string name)
	{
		_FunctionToTime t;
		_mutex.lock();
		_timedFuncs.try_emplace(name, t);
		_timedFuncs[name]._name = name;
		_timedFuncs[name]._start = std::chrono::high_resolution_clock::now();
		_timedFuncs[name]._ticks++;
		_mutex.unlock();
	};
	void PerformanceTimer::Stop(const std::string name)
	{
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
		_mutex.lock();
		try
		{
			_timedFuncs.at(name)._time = (double)(duration(timeNow() - _timedFuncs.at(name)._start));
			_timedFuncs.at(name)._cumulative += _timedFuncs[name]._time;
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

}