#pragma once

#include "GameRendererBase.h"

namespace game
{
	class Engine;
	extern Engine* enginePointer;
	// Engine class Start
	class Engine
	{
	public:
		bool isRunning = false;
		Engine()
		{
			enginePointer = this;
			renderer = nullptr;
		}
		Window window;
		RendererBase* renderer;
	private:
	};
	// --- Engine class Stop
}