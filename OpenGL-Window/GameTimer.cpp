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

	float GameTimer::Elapsed()
	{
		std::chrono::microseconds ticks = std::chrono::duration_cast<std::chrono::microseconds>((std::chrono::high_resolution_clock::now() - _startTime));
		float diff = (float)(ticks.count() / 1000.0f);
		return diff;
	}

}
