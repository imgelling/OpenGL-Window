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

		void SetWindowTitle(const std::string title);
	private:
#pragma region NeedsToBeCreatedByUser
		void Update(const double msElapsed);
		void Render(const double msElapsed);
		void Initialize();
		void Shutdown();
		void Swap();
#pragma endregion
		double _frameTime;
		GameAttributes _attributes;
		RendererBase* _renderer;
		Window _window;
		GameTimer _timer;
		GameTimer _frameLockTimer;
	};
}