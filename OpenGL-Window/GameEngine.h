#pragma once

namespace game
{
	class Engine;
	Engine* enginePointer;
	// Engine class Start
	class Engine
	{
	public:
		bool isRunning = false;
		Engine()
		{
			enginePointer = this;
			//r = nullptr;
		}
		Window window;
		//RendererBase* r;
	private:
	};
	// --- Engine class Stop
}