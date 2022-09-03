#pragma once
#include <chrono>

namespace game
{
	class Timer
	{
	public:
		Timer();
		float Elapsed();
		void Reset();
	private:
		std::chrono::high_resolution_clock::time_point _startTime;
	};
}

