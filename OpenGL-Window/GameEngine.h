#pragma once

#include "GameRendererBase.h"
#include "GameAttributes.h"
#include "GameTimer.h"
#include "GameLogger.h"
#include "GameKeyboard.h"
#include "GameMouse.h"
#include "GameSystemInfo.h"
#include "GameRendererGL.h"
#include "GameRendererVK.h"
#include "GameTexture2D.h"
//#include "GameTerminal.h"

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
	public:
		Keyboard keyboard;
		Mouse mouse;
		Logger* logger;
		bool isRunning;

		Engine(Logger* logger);
		~Engine();
		void SetAttributes(const Attributes &attrib);
		bool Create();
		void StartEngine();
		void StopEngine();

		void SetWindowTitle(const std::string title);
		void SetFrameLock(const uint32_t limit);
		uint32_t GetUpdatesPerSecond();
		uint32_t GetFramesPerSecond();
		
		bool LoadTexture(std::string fileName, Texture2dGL &texture);
		void UnLoadTexture(Texture2dGL& texture);
		bool LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader);
		void UnLoadShader(ShaderGL& shader);
		
		void ToggleFullscreen();
		void HandleWindowResize(const uint32_t width, const uint32_t height);
		virtual void Update(const float_t msElapsed) = 0;
		virtual void Render(const float_t msElapsed) = 0;
		virtual void Initialize() = 0;
		virtual void LoadContent() = 0;
		virtual void Shutdown() = 0;

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
		uint32_t _updatesPerSecond;
		uint32_t _framesPerSecond;
	};


	inline Engine::Engine(Logger* logger)
	{
		isRunning = false;
		enginePointer = this;
		_renderer = nullptr;
		_frameTime = 0.0f;
		_updatesPerSecond = 0;
		_framesPerSecond = 0;
		this->logger = logger;
	}

	inline Engine::~Engine()
	{
		if (_renderer) _renderer->DestroyDevice();
		delete _renderer;
	}

	inline void Engine::StartEngine()
	{
		// Storage of time
		float_t msElapsed = 0.0f;
		// Tracks updates per second
		double_t upsTime = 0.0f;
		uint32_t updatesCounted = 0;
		// Tracks frames per second
		float_t fpsTime = 0.0f;
		uint32_t framesCounted = 0;


		isRunning = true;

		// Reset the timers
		_renderTimer.Reset();
		_frameLockTimer.Reset();
		_updateTimer.Reset();

		// Do the game loop
		do
		{
			mouse.ResetMouseValues();

			// Do window messages
			_ProcessMessages();

			// Try to update as fast as possible and keep track of UPS
			msElapsed = _updateTimer.Elapsed();
			if (msElapsed > (0.0f))
			{
				Update(msElapsed);
				_updateTimer.Reset();
				upsTime += msElapsed;
				updatesCounted++;
				if (upsTime >= 1000.0f)
				{
					_updatesPerSecond = updatesCounted;
					updatesCounted = 0;
					upsTime = upsTime - 1000.0f;
				}
			}

			// If software frame lock is on, make sure we adhere to that
			// and keep track of FPS
			if (_frameLockTimer.Elapsed() >= _frameTime)
			{
				_frameLockTimer.Reset();
				msElapsed = _renderTimer.Elapsed();
				_renderTimer.Reset();
				Render(msElapsed);
				fpsTime += msElapsed;
				framesCounted++;
				if (fpsTime >= 1000.0f)
				{
					_framesPerSecond = framesCounted;
					framesCounted = 0;
					fpsTime = fpsTime - 1000.0f;
				}

				// Swap the buffers
				_Swap();
			}

		} while (isRunning);

		// Clean up end user stuff
		Shutdown();
	}

	inline void Engine::StopEngine()
	{
		isRunning = false;
	}

	inline void Engine::_ProcessMessages()
	{
		_window.DoMessagePump();
	}

	inline void Engine::SetAttributes(const Attributes& attrib)
	{
		_attributes = attrib;
		if (_attributes.Framelock > 0)
		{
			_frameTime = 1000.0f / _attributes.Framelock;
		}
		else
		{
			_frameTime = 0.0f;
		}
	}

	inline void Engine::SetFrameLock(const uint32_t limit)
	{
		_attributes.Framelock = (float)limit;
		if (_attributes.Framelock > 0)
		{
			_frameTime = 1000.0f / _attributes.Framelock;
		}
		else
		{
			_frameTime = 0.0f;
		}

	}

	inline uint32_t Engine::GetUpdatesPerSecond()
	{
		return _updatesPerSecond;
	}

	inline uint32_t Engine::GetFramesPerSecond()
	{
		return _framesPerSecond;
	}

	inline bool Engine::LoadTexture(std::string fileName, Texture2dGL &texture)
	{
		return _renderer->LoadTexture(fileName, texture);
	}

	inline void Engine::UnLoadTexture(Texture2dGL& texture)
	{
		_renderer->UnLoadTexture(texture);
	}

	inline bool Engine::LoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader)
	{
		return _renderer->LoadShader(vertex, fragment, shader);
	}

	inline void Engine::UnLoadShader(ShaderGL& shader)
	{
		_renderer->UnLoadShader(shader);
	}

	inline void Engine::SetWindowTitle(const std::string title)
	{
		_window.SetWindowTitle(title);
		_attributes.WindowTitle = title;
	}

	inline void Engine::ToggleFullscreen()
	{
		_window.ToggleFullScreen();
		_attributes.WindowFullscreen = !_attributes.WindowFullscreen;
	}

	inline bool Engine::Create()
	{
		// Let user choose how they want things
		Initialize();

		logger->Header(_attributes.WindowTitle, _attributes.GameVersion);

		// Create the window
		_window.SetAttributes(_attributes);
		if (!_window.CreateTheWindow())
		{
			std::cout << game::lastError;
			return false;
		}

		// Set the renderer
		if (_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			_renderer = new game::RendererGL();
		}
		else if (_attributes.RenderingAPI == RenderAPI::Vulkan)
		{
			_renderer = new game::RendererVK();
		}
		else
		{
			lastError = { GameErrors::GameInvalidParameter, "Only OpenGL is implemented." };
			return false;
		}
		_renderer->SetAttributes(_attributes, logger);

		// Create rendering device
		if (!_renderer->CreateDevice(_window))
		{
			_renderer->DestroyDevice();
			return false;
		}

		_renderer->FillOutRendererInfo();

		// Load user content
		LoadContent();

		return true;
	}

	inline void Engine::_Swap()
	{
		if (_renderer)
		{
			_renderer->Swap();
		}
	}

	inline void Engine::HandleWindowResize(const uint32_t width, const uint32_t height)
	{
		if (_renderer)
		{
			_renderer->HandleWindowResize(width, height);
		}
	}

	// Needs to be in engine.h because of cyclic dependency by using enginePointer.
	inline LRESULT CALLBACK Window::_WindowEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_MOUSEMOVE: 	enginePointer->mouse.HandleMouseMove(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF); return 0;
		case WM_MOUSEWHEEL:	enginePointer->mouse.HandleMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam)); return 0;
			//case WM_MOUSELEAVE: ptrPGE->olc_UpdateMouseFocus(false);                                    return 0;
			//case WM_SETFOCUS:	ptrPGE->olc_UpdateKeyFocus(true);                                       return 0;
			//case WM_KILLFOCUS:	ptrPGE->olc_UpdateKeyFocus(false);                                      return 0;
		case WM_LBUTTONDOWN:enginePointer->mouse.SetMouseState(0, true); return 0;
		case WM_LBUTTONUP:	enginePointer->mouse.SetMouseState(0, false); return 0;
		case WM_RBUTTONDOWN:enginePointer->mouse.SetMouseState(2, true); return 0;
		case WM_RBUTTONUP:	enginePointer->mouse.SetMouseState(2, false); return 0;
		case WM_MBUTTONDOWN:enginePointer->mouse.SetMouseState(1, true); return 0;
		case WM_MBUTTONUP:	enginePointer->mouse.SetMouseState(1, false); return 0;
		case WM_SIZE: enginePointer->HandleWindowResize(lParam & 0xFFF, (lParam >> 16) & 0xFFFF); return 0;
		case WM_KEYDOWN: enginePointer->keyboard.SetKeyState((uint8_t)wParam, true); return 0;
		case WM_KEYUP: enginePointer->keyboard.SetKeyState((uint8_t)wParam, false); return 0;
			//case WM_SYSKEYDOWN: ptrPGE->olc_UpdateKeyState(mapKeys[wParam], true);						return 0;
			//case WM_SYSKEYUP:	ptrPGE->olc_UpdateKeyState(mapKeys[wParam], false);
		case WM_CLOSE:		if (enginePointer) enginePointer->isRunning = false; return 0;
		case WM_DESTROY:	PostQuitMessage(0); DestroyWindow(hWnd); return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}