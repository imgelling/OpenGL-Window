#pragma once
#include <random>
#include <chrono>

namespace game
{
	class GameRandom
	{
	public:
		GameRandom();
		void SetSeed(const unsigned int);
		uint32_t GetSeed();
		uint32_t Rnd();
		uint32_t RndRange(const unsigned int, const unsigned int);
		~GameRandom();
	private:
		unsigned int seed;
		std::mt19937 generator;

	};

	GameRandom::GameRandom()
	{
		std::random_device rd;
		seed = rd() ^
			(
				(std::mt19937::result_type)
				std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() +
				(std::mt19937::result_type)
				std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()
				);
		generator.seed(seed);
	}

	inline void GameRandom::SetSeed(const unsigned int seed)
	{
		this->seed = seed;
		generator.seed(seed);
	}

	inline uint32_t GameRandom::GetSeed()
	{
		return seed;
	}

	inline uint32_t GameRandom::Rnd()
	{
		return generator();
	}

	inline uint32_t GameRandom::RndRange(const unsigned int min, const unsigned int max)
	{
		std::uniform_int_distribution<unsigned> distrib(min, max);
		return distrib(generator);
	}

	GameRandom::~GameRandom()
	{
	}
}