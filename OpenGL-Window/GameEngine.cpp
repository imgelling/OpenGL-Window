#include "GameEngine.h"

namespace game
{
	Engine::Engine()
	{
		isRunning = false;
		enginePointer = this;
		renderer = nullptr;
	}

	Engine::~Engine()
	{
		delete renderer;
	}
}