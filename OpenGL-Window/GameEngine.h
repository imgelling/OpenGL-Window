#pragma once

#include "GameRendererBase.h"
#include "GameAttributes.h"
#include "GameTimer.h"
#include "GameLogger.h"
#include "GameKeyboard.h"

namespace game
{
	// Makes computer choose dedicated over integrated gpus
#ifdef GAME_USE_DEDICATED_GPU
	extern "C"
	{
		__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	} // can be slower for some reason
#endif

	class Engine;
	extern Engine* enginePointer;
	
	class Engine
	{
		//friend Window;
	public:
		bool isRunning;
		GameLogger* logger;
		Keyboard keyboard;

		Engine(GameLogger* logger);
		~Engine();
		void SetAttributes(const Attributes &attrib);
		bool Create();
		void Start();
		void Stop();

		void SetWindowTitle(const std::string title);
		void SetFrameLock(const uint32_t limit);
		void ToggleFullscreen();
		void HandleWindowResize(const uint32_t width, const uint32_t height);
#pragma region NeedsToBeCreatedByUser
		virtual void Update(const float msElapsed) {};
		virtual void Render(const float msElapsed) {};
		virtual void Initialize() {};
		virtual void Shutdown() {};
#pragma endregion
	private:
		float _frameTime;
		Attributes _attributes;
		RendererBase* _renderer;
		Window _window;
		Timer _renderTimer;
		Timer _updateTimer;
		Timer _frameLockTimer;
		void _ProcessMessages();
		void _Swap();


	};
}