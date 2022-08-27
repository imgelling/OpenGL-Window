#pragma once

#include "GameRendererBase.h"
#include "GameAttributes.h"
#include "GameTimer.h"
#include "GameLogger.h"

namespace game
{
	class Engine;
	extern Engine* enginePointer;
	
	class Engine
	{
	public:
		bool isRunning;
		GameLogger* logger;

		Engine(GameLogger* logger);
		~Engine();
		void ProcessMessages();
		void SetAttributes(const GameAttributes &attrib);
		bool Create();
		void Start();

		void SetWindowTitle(const std::string title);
		void SetFrameLock(const uint32_t limit);
	private:
#pragma region NeedsToBeCreatedByUser
		void Update(const float msElapsed);
		void Render(const float msElapsed);
		void Initialize();
		void Shutdown();
#pragma endregion
		void Swap();
		float _frameTime;
		GameAttributes _attributes;
		RendererBase* _renderer;
		Window _window;
		GameTimer _timer;
		GameTimer _frameLockTimer;


	};
}