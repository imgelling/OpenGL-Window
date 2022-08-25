#pragma once

#include "GameRendererBase.h"
#include "GameAttributes.h"

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

		void ProcessMessages();
		void SetAttributes(const GameAttributes &attrib);
		bool Start();
		void Swap();

		void Initialize();
		void Shutdown();
	private:
		GameAttributes _attributes;
	};
}