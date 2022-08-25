#pragma once

#include "GameRendererBase.h"

namespace game
{
	class Engine;
	extern Engine* enginePointer;
	
	class Engine
	{
	public:
		bool isRunning;
		RendererBase* renderer;
		Window window;

		Engine();
		~Engine();

		void Initialize();
		void Shutdown();
	private:
	};
}