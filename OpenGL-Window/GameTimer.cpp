#include "GameTimer.h"

namespace game
{
	Timer::Timer()
	{
		_startTime = std::chrono::high_resolution_clock::now();
	}
	
	void Timer::Reset()
	{
		_startTime = std::chrono::high_resolution_clock::now();
	}

	float_t Timer::Elapsed()
	{
		std::chrono::microseconds _ticks = std::chrono::duration_cast<std::chrono::microseconds>((std::chrono::high_resolution_clock::now() - _startTime));
		float_t diff = (float)(_ticks.count()) / 1000.0f;
		return diff;
	}

}
