#include "GameTimer.h"

namespace game
{
	GameTimer::GameTimer()
	{
		_startTime = std::chrono::high_resolution_clock::now();
	}
	
	void GameTimer::Reset()
	{
		_startTime = std::chrono::high_resolution_clock::now();
	}

	double GameTimer::Elapsed()
	{
		std::chrono::microseconds ticks = std::chrono::duration_cast<std::chrono::microseconds>((std::chrono::high_resolution_clock::now() - _startTime));
		double diff = (double)(ticks.count()) / 1000.0;
		return diff;
	}

}
