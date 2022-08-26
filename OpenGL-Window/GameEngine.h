#pragma once

#include "GameRendererBase.h"
#include "GameAttributes.h"
#include "GameTimer.h"

namespace game
{
	class Engine;
	extern Engine* enginePointer;
	
	class Engine
	{
	public:
		bool isRunning;

		Engine();
		~Engine();
		void ProcessMessages();
		void SetAttributes(const GameAttributes &attrib);
		bool Create();
		void Start();
	private:
#pragma region NeedsToBeCreatedByUser
		void Update(const float msElapsed);
		void Render(const float msElapsed);
		void Initialize();
		void Shutdown();
		void Swap();
#pragma endregion
		GameAttributes _attributes;
		RendererBase* _renderer;
		Window _window;
		GameTimer _timer;
	};
}