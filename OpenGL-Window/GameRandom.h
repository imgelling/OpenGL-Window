#pragma once
#include <random>
#include <chrono>

namespace game
{
	class GameRandom
	{
	public:
		GameRandom();
		void SetSeed(const uint32_t seed);
		uint32_t GetSeed();
		uint32_t Rnd();
		uint32_t RndRange(const uint32_t min, const uint32_t max);
		~GameRandom();
	private:
		unsigned int _seed;
		std::mt19937 _mt19937Generator;

	};

	GameRandom::GameRandom()
	{
		std::random_device rd;
		_seed = rd() ^
			(
				(std::mt19937::result_type)
				std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() +
				(std::mt19937::result_type)
				std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
				);
		_mt19937Generator.seed(_seed);
	}

	inline void GameRandom::SetSeed(const uint32_t seed)
	{
		this->_seed = seed;
		_mt19937Generator.seed(seed);
	}

	inline uint32_t GameRandom::GetSeed()
	{
		return _seed;
	}

	inline uint32_t GameRandom::Rnd()
	{
		return _mt19937Generator();
	}

	inline uint32_t GameRandom::RndRange(const uint32_t min, const uint32_t max)
	{
		std::uniform_int_distribution<unsigned> distrib(min, max);
		return distrib(_mt19937Generator);
	}

	GameRandom::~GameRandom()
	{
	}
}