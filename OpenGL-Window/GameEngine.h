#pragma once

#include "GameRendererBase.h"
#include "GameAttributes.h"
#include "GameTimer.h"
#include "GameLogger.h"
#include "GameKeyboard.h"
#include "GameMouse.h"
#include "GameSystemInfo.h"
#include "GameDefines.h"

#pragma region Vulkan
#if defined(GAME_SUPPORT_VULKAN) || defined(GAME_SUPPORT_ALL)
#include "GameRendererVK.h"
#endif
#pragma endregion

#pragma region DirectX9
#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL)
#include "GameRendererDX9.h"
#endif
#pragma endregion

#pragma region Opengl
#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
#include "GameRendererGL.h"
#endif
#pragma endregion


#include "GameTexture2D.h"
#include "GameMath.h"





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
	extern SystemInfo systemInfo;
	
	class Engine
	{
		friend class PixelMode;
		friend class SpriteBatch;
	public:
		Keyboard geKeyboard;
		Mouse geMouse;
		Logger* geLogger;
		bool geIsRunning;

		// Engine setup

		Engine(Logger* logger);
		~Engine();
		void geSetAttributes(const Attributes &attrib);
		bool geCreate();
		void geStartEngine();
		void geStopEngine();

		// Frame and update timing 
		
		uint32_t geGetUpdatesPerSecond() const noexcept;
		uint32_t geGetFramesPerSecond() const noexcept;
		uint32_t geGetCPUFrequency() const noexcept;
		void geSetFrameLock(const uint32_t limit) noexcept;
		
		// Renderer specific
		
		bool geCreateTexture(Texture2D& texture);
		bool geLoadTexture(const std::string fileName, Texture2D& texture);
		void geUnLoadTexture(Texture2D& texture);
		bool geLoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader);
		void geUnLoadShader(ShaderGL& shader);
		void geSetClearColor(const Color& color) noexcept;
		void geClear(const bool Color, const bool Depth, const bool Stencil) noexcept;
		void geEnable(const uint32_t capability) noexcept;
		void geDisable(const uint32_t capability) noexcept;
		void geBindTexture(const uint32_t capability, const Texture2D& texture) noexcept;

		// Window stuff	

		Vector2i geGetWindowSize() const noexcept;
		void geSetWindowTitle(const std::string title);
		void geToggleFullscreen();
		void HandleWindowResize(const uint32_t width, const uint32_t height);
		
		// Created by end user		

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
		Timer _cpuSpeedTimer;
		uint32_t _cpuFrequency;
		uint32_t _updatesPerSecond;
		uint32_t _framesPerSecond;
		void _GetAndLogCPUInfo();
		void _ProcessMessages() noexcept;
		void _Swap();
	};


	inline Engine::Engine(Logger* logger)
	{
		geIsRunning = false;
		enginePointer = this;
		_renderer = nullptr;
		_frameTime = 0.0f;
		_updatesPerSecond = 0;
		_framesPerSecond = 0;
		_cpuFrequency = 0;
		this->geLogger = logger;
	}

	inline Engine::~Engine()
	{
		if (_renderer)
		{

			_renderer->DestroyDevice();
			delete _renderer;
		}
	}

	inline void Engine::geStartEngine()
	{
		// Storage of time
		float_t msElapsed = 0.0f;
		// Tracks updates per second
		double_t upsTime = 0.0f;
		uint32_t updatesCounted = 0;
		// Tracks frames per second
		float_t fpsTime = 0.0f;
		uint32_t framesCounted = 0;
		__int64 cyclesStart = __rdtsc();

		geIsRunning = true;

		// Reset the timers
		_renderTimer.Reset();
		_frameLockTimer.Reset();
		_updateTimer.Reset();
		_cpuSpeedTimer.Reset();

		// Do the game loop
		do
		{
			geMouse.ResetMouseValues();

			// Do window messages
			_ProcessMessages();
			if (!geIsRunning) break;

			// Update cpu frequency
			if (_cpuSpeedTimer.Elapsed() > 1000.0f)
			{
				_cpuSpeedTimer.Reset();
				_cpuFrequency = (uint32_t)((__rdtsc() - cyclesStart) / 1000000);
				cyclesStart = __rdtsc();
			}


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

		} while (geIsRunning);

		// Clean up end user stuff
		Shutdown();
	}

	inline void Engine::geStopEngine()
	{
		geIsRunning = false;
	}

	inline void Engine::_ProcessMessages() noexcept
	{
		_window.DoMessagePump();
	}

	inline void Engine::geSetAttributes(const Attributes& attrib)
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

	inline void Engine::geSetFrameLock(const uint32_t limit) noexcept
	{
		_attributes.Framelock = (float_t)limit;
		if (_attributes.Framelock > 0)
		{
			_frameTime = 1000.0f / _attributes.Framelock;
		}
		else
		{
			_frameTime = 0.0f;
		}

	}

	inline uint32_t Engine::geGetCPUFrequency() const noexcept
	{
		return _cpuFrequency;
	}

	inline uint32_t Engine::geGetUpdatesPerSecond() const noexcept
	{
		return _updatesPerSecond;
	}

	inline uint32_t Engine::geGetFramesPerSecond() const noexcept
	{
		return _framesPerSecond;
	}

	inline Vector2i Engine::geGetWindowSize() const noexcept
	{
		return { (int)_attributes.WindowWidth, (int)_attributes.WindowHeight};
	}

	inline void Engine::geSetClearColor(const Color &color) noexcept
	{
		if (_renderer)
		{
			_renderer->SetClearColor(color);
		}
	}
	
	inline void Engine::geClear(const bool color, const bool depth, const bool stencil) noexcept
	{
		if (_renderer)
		{
			_renderer->Clear(color, depth, stencil);
		}
	}

	inline bool Engine::geCreateTexture(Texture2D& texture)
	{
		if (_renderer)
		{
			return _renderer->CreateTexture(texture);
		}
		return false;
	}

	inline bool Engine::geLoadTexture(const std::string fileName, Texture2D &texture)
	{
		if (_renderer)
		{
			return _renderer->LoadTexture(fileName, texture);
		}
		return false;
	}

	inline void Engine::geUnLoadTexture(Texture2D& texture)
	{
		if (_renderer)
		{
			_renderer->UnLoadTexture(texture);
		}
	}

	inline bool Engine::geLoadShader(const std::string vertex, const std::string fragment, ShaderGL& shader)
	{
		if (_renderer)
		{
			return _renderer->LoadShader(vertex, fragment, shader);
		}
		return false;
	}

	inline void Engine::geUnLoadShader(ShaderGL& shader)
	{
		if (_renderer)
		{
			_renderer->UnLoadShader(shader);
		}
	}

	inline void Engine::geSetWindowTitle(const std::string title)
	{
		_window.SetWindowTitle(title);
		_attributes.WindowTitle = title;
	}

	inline void Engine::geToggleFullscreen()
	{
		_window.ToggleFullScreen();
		_attributes.WindowFullscreen = !_attributes.WindowFullscreen;
	}

	inline void Engine::geEnable(const uint32_t capability) noexcept
	{
		if (_renderer)
		{
			_renderer->Enable(capability);
		}
	}
	
	inline void Engine::geDisable(const uint32_t capability) noexcept
	{
		if (_renderer)
		{
			_renderer->Disable(capability);
		}
	}

	inline void Engine::geBindTexture(const uint32_t capability, const Texture2D &texture) noexcept
	{
		if (_renderer)
		{
			_renderer->BindTexture(capability, texture);
		}
	}

	inline void Engine::_GetAndLogCPUInfo()
	{
		std::stringstream sStream;

		// Lambda to log easier
		auto LOG = [&](std::stringstream& stream)
		{
			geLogger->Write(stream.str());
			stream.str("");
		};

		systemInfo.GetCPUInfo();

		sStream << "CPU vendor : " << systemInfo.cpuInfo.processorVendor;
		LOG(sStream);

		sStream << "CPU name : " << systemInfo.cpuInfo.processorName;
		LOG(sStream);

		sStream << "CPU processor thread count : " << systemInfo.cpuInfo.processorCount;
		LOG(sStream);

		sStream << "RAM physical total : " << systemInfo.ramInfo.totalPhysicalMemory << "MBs";
		LOG(sStream);

		sStream << "RAM free physical total : " << systemInfo.ramInfo.totalPhysicalMemoryAvailable << "MBs";
		LOG(sStream);
		
	}

	inline bool Engine::geCreate()
	{
		// Let user choose how they want things
		Initialize();

		geLogger->Header(_attributes.WindowTitle, _attributes.GameVersion);

		// Get and log the cpu info
		_GetAndLogCPUInfo();

		// Create the window
		_window.SetAttributes(_attributes);
		if (!_window.CreateTheWindow())
		{
			return false;
		}

		// Set the renderer
		if (_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
			_renderer = new game::RendererGL();
#else
			lastError = { GameErrors::GameInvalidParameter, "Requested OpenGL without #defining GAME_SUPPORT_OPENGL or GAME_SUPPORT ALL." };
			return false;
#endif
		}
		else if (_attributes.RenderingAPI == RenderAPI::Vulkan)
		{
			lastError = { GameErrors::GameInvalidParameter, "Only OpenGL is implemented." };
#if defined(GAME_VULKAN)
			_renderer = new game::RendererVK;
#else
			lastError = { GameErrors::GameInvalidParameter, "Requested Vulkan without #defining GAME_SUPPORT_VULKAN or GAME_SUPPORT ALL." };
			return false;
#endif
			return false;
		}
		else if (_attributes.RenderingAPI == RenderAPI::DirectX9)
		{
			//lastError = { GameErrors::GameInvalidParameter, "Starting to implement" };
#if defined(GAME_DIRECTX9)
			_renderer = new game::RendererDX9();
#else
			lastError = { GameErrors::GameInvalidParameter, "Requested DirectX9 without #defining GAME_SUPPORT_DIRECTX9 or GAME_SUPPORT ALL." };
			return false;
#endif
		}
		else
		{
			lastError = { GameErrors::GameInvalidParameter, "Only OpenGL is implemented." };
			return false;
		}
		_renderer->SetAttributes(_attributes, geLogger);

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
		_attributes.WindowWidth = width;
		_attributes.WindowHeight = height;
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
		case WM_MOUSEMOVE: 	enginePointer->geMouse.HandleMouseMove(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF); return 0;
		case WM_MOUSEWHEEL:	enginePointer->geMouse.HandleMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam)); return 0;
			//case WM_MOUSELEAVE: ptrPGE->olc_UpdateMouseFocus(false);                                    return 0;
			//case WM_SETFOCUS:	ptrPGE->olc_UpdateKeyFocus(true);                                       return 0;
			//case WM_KILLFOCUS:	ptrPGE->olc_UpdateKeyFocus(false);                                      return 0;
		case WM_LBUTTONDOWN:enginePointer->geMouse.SetMouseState(0, true); return 0;
		case WM_LBUTTONUP:	enginePointer->geMouse.SetMouseState(0, false); return 0;
		case WM_RBUTTONDOWN:enginePointer->geMouse.SetMouseState(2, true); return 0;
		case WM_RBUTTONUP:	enginePointer->geMouse.SetMouseState(2, false); return 0;
		case WM_MBUTTONDOWN:enginePointer->geMouse.SetMouseState(1, true); return 0;
		case WM_MBUTTONUP:	enginePointer->geMouse.SetMouseState(1, false); return 0;
		case WM_SIZE: enginePointer->HandleWindowResize(lParam & 0xFFF, (lParam >> 16) & 0xFFFF); return 0;
		case WM_KEYDOWN: enginePointer->geKeyboard.SetKeyState((uint8_t)wParam, true); return 0;
		case WM_KEYUP: enginePointer->geKeyboard.SetKeyState((uint8_t)wParam, false); return 0;
			//case WM_SYSKEYDOWN: ptrPGE->olc_UpdateKeyState(mapKeys[wParam], true);						return 0;
			//case WM_SYSKEYUP:	ptrPGE->olc_UpdateKeyState(mapKeys[wParam], false);
		case WM_CLOSE:		if (enginePointer) enginePointer->geIsRunning = false; return 0;
		case WM_DESTROY:	PostQuitMessage(0); DestroyWindow(hWnd); return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}