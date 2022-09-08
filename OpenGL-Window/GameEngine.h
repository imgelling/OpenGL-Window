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
		Logger* logger;
		Keyboard keyboard;

		Engine(Logger* logger);
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
		virtual void Update(const float_t msElapsed) = 0;
		virtual void Render(const float_t msElapsed) = 0;
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
#pragma endregion
	private:
		float_t _frameTime;
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