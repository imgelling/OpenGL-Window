#pragma once

#include "GameAttributes.h"
#include "GameRendererBase.h"
#include "GameKeyboard.h"
#include "GameLogger.h"
#include "GameMouse.h"
#include "GameSystemInfo.h"
#include "GameTimer.h"

#pragma region Vulkan
#if defined(GAME_VULKAN)
#include "GameRendererVK.h"
#endif
#pragma endregion

#pragma region DirectX9
#if defined(GAME_DIRECTX9)
#include "GameRendererDX9.h"
#endif
#pragma endregion

#pragma region Opengl
#if defined(GAME_OPENGL)
#include "GameRendererGL.h"
#endif
#pragma endregion

#pragma region DirectX11
#if defined(GAME_DIRECTX11)
#include "GameRendererDX11.h"
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
	}
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
		bool geIsMinimized;
		bool test;
#if defined(GAME_DIRECTX9)
		LPDIRECT3DDEVICE9 d3d9Device;
#endif
#if defined(GAME_DIRECTX11)
		ID3D11DeviceContext* d3d11Context;
		ID3D11RenderTargetView* d3d11RenderTarget;
		ID3D11Device* d3d11Device;
#endif

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
		void geSetUpdateLock(const uint32_t limit) noexcept;
		void geSetGameLocks(const uint32_t fps, const uint32_t ups) noexcept;
		
		// Renderer specific
		
		bool geCreateTexture(Texture2D& texture);
		bool geLoadTexture(const std::string fileName, Texture2D& texture);
		void geUnLoadTexture(Texture2D& texture);
		bool geLoadShader(const std::string vertex, const std::string fragment, Shader& shader);
		void geUnLoadShader(Shader& shader);
		bool geIsUsing(const uint32_t renderer);

		// Window stuff	

		Vector2i geGetWindowSize() const noexcept;
		void geSetWindowTitle(const std::string title);
		void geToggleFullscreen();
		void HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset);
		
		// Created by end user		

		virtual void Update(const float_t msElapsed) = 0;
		virtual void Render(const float_t msElapsed) = 0;
		virtual void Initialize() = 0;
		virtual void LoadContent() = 0;
		virtual void Shutdown() = 0;

		virtual void HandleWindowSizeChange() {}

		// Tools
		void geLogLastError();

	private:
		float_t _frameTime;
		float_t _updateTime;
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
		_updateTime = 0.0f;
		_updatesPerSecond = 0;
		_framesPerSecond = 0;
		_cpuFrequency = 0;
		this->geLogger = logger;
		geIsMinimized = false;
		test = false;
#if defined(GAME_DIRECTX9)
		d3d9Device = nullptr;
#endif
#if defined(GAME_DIRECTX11)
		d3d11Context = nullptr;
		d3d11RenderTarget = nullptr;
		d3d11Device = nullptr;
#endif
	}

	inline Engine::~Engine()
	{
		if (_renderer)
		{
			_renderer->DestroyDevice();
			delete _renderer;
		}
#if defined(GAME_DIRECTX9)
		if (d3d9Device)
		{
			d3d9Device->Release();
		}
#endif
#if defined(GAME_DIRECTX11)
		if (d3d11RenderTarget)
		{
			d3d11RenderTarget->Release();
		}
		if (d3d11Context)
		{
			d3d11Context->Release();
		}
		if (d3d11Device)
		{
			d3d11Device->Release();
		}
#endif
	}

	inline bool Engine::geIsUsing(const uint32_t renderer)
	{
#if defined(GAME_OPENGL)
		if ((renderer == 1) && (_attributes.RenderingAPI == RenderAPI::OpenGL))
			return true;
#endif
#if defined(GAME_DIRECTX9)
		if ((renderer == 2) && (_attributes.RenderingAPI == RenderAPI::DirectX9))
			return true;
#endif
#if defined(GAME_VULKAN)
		if ((renderer == 3) && (_attributes.RenderingAPI == RenderAPI::Vulkan))
			return true;
#endif
#if defined(GAME_DIRECTX11)
		if ((renderer == 4) && (_attributes.RenderingAPI == RenderAPI::DirectX11))
			return true;
#endif
#if defined(GAME_DIRECTX12)
		if ((renderer == 5) && (_attributes.RenderingAPI == RenderAPI::DirectX12))
			return true;
#endif
		return false;
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

			if (geIsMinimized)
			{
				Sleep((DWORD)(_updateTime));
			}

			// Update cpu frequency
			if (_cpuSpeedTimer.Elapsed() > 1000.0f)
			{
				_cpuSpeedTimer.Reset();
				_cpuFrequency = (uint32_t)((__rdtsc() - cyclesStart) / 1000000);
				cyclesStart = __rdtsc();
			}

			// Update to updatelock
			msElapsed = _updateTimer.Elapsed();
			if (msElapsed >= (_updateTime))
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
		if (_attributes.FrameLock > 0)
		{
			_frameTime = 1000.0f / _attributes.FrameLock;
		}
		else
		{
			_frameTime = 0.0f;
		}
		if (_attributes.UpdateLock > 0)
		{
			_updateTime = 1000.0f / _attributes.UpdateLock;
		}
		else
		{
			_updateTime = 0.0f;
		}
	}

	inline void Engine::geSetFrameLock(const uint32_t limit) noexcept
	{
		_attributes.FrameLock = (float_t)limit;
		if (_attributes.FrameLock > 0)
		{
			_frameTime = 1000.0f / _attributes.FrameLock;
		}
		else
		{
			_frameTime = 0.0f;
		}
	}

	inline void Engine::geSetUpdateLock(const uint32_t limit) noexcept
	{
		_attributes.UpdateLock = (float_t)limit;
		if (_attributes.UpdateLock > 0)
		{
			_updateTime = 1000.0f / _attributes.UpdateLock;
		}
		else
		{
			_updateTime = 0.0f;
		}
	}

	inline void Engine::geSetGameLocks(const uint32_t fps, const uint32_t ups) noexcept
	{
		geSetFrameLock(fps);
		geSetUpdateLock(ups);
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

	inline bool Engine::geLoadShader(const std::string vertex, const std::string fragment, Shader& shader)
	{
		if (_renderer)
		{
			return _renderer->LoadShader(vertex, fragment, shader);
		}
		return false;
	}

	inline void Engine::geUnLoadShader(Shader& shader)
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
#if defined(GAME_OPENGL)
			_renderer = new game::RendererGL();
#else
			lastError = { GameErrors::GameInvalidParameter, "Requested OpenGL without #defining GAME_SUPPORT_OPENGL or GAME_SUPPORT ALL." };
			return false;
#endif
		}
		else if (_attributes.RenderingAPI == RenderAPI::Vulkan)
		{
			lastError = { GameErrors::GameInvalidParameter, "Not implemented." };
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
#if defined(GAME_DIRECTX9)
			_renderer = new game::RendererDX9();
#else
			lastError = { GameErrors::GameInvalidParameter, "Requested DirectX9 without #defining GAME_SUPPORT_DIRECTX9 or GAME_SUPPORT ALL." };
			return false;
#endif
		}
		else if (_attributes.RenderingAPI == RenderAPI::DirectX11)
		{
#if defined(GAME_DIRECTX11)
			_renderer = new game::RendererDX11();
#else
			lastError = { GameErrors::GameInvalidParameter, "Requested DirectX11 without #defining GAME_SUPPORT_DIRECTX11 or GAME_SUPPORT ALL." };
			return false;
#endif
		}
		else if (_attributes.RenderingAPI == RenderAPI::DirectX12)
		{
#if defined(GAME_DIRECTX12)
			_renderer = new game::RendererDX12();
#else
			lastError = { GameErrors::GameInvalidParameter, "Requested DirectX12 without #defining GAME_SUPPORT_DIRECTX12 or GAME_SUPPORT ALL." };
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

#if defined(GAME_DIRECTX9)
		if (geIsUsing(GAME_DIRECTX9))
		{
			if (_renderer)
			{
				dynamic_cast<RendererDX9*>(_renderer)->GetDevice(d3d9Device);
			}
		}
#endif

#if defined(GAME_DIRECTX11)
		if (geIsUsing(GAME_DIRECTX11))
		{
			if (_renderer)
			{
				dynamic_cast<RendererDX11*>(_renderer)->GetDevice(d3d11Device, d3d11Context, d3d11RenderTarget);
			}
		}
#endif

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

	inline void Engine::HandleWindowResize(const uint32_t width, const uint32_t height, const bool doReset)
	{
		_attributes.WindowWidth = width;
		_attributes.WindowHeight = height;
		if (_renderer)
		{
			_renderer->HandleWindowResize(width, height, doReset);
		}
		HandleWindowSizeChange();
	}

	inline void Engine::geLogLastError()
	{
		geLogger->Error(lastError);
	}

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
		case WM_SIZING: break;
		case WM_SIZE: 
		{
			switch (wParam)
			{
			case SIZE_MINIMIZED:
				enginePointer->geIsMinimized = true;
				enginePointer->HandleWindowResize(lParam & 0xFFF, (lParam >> 16) & 0xFFFF, true); return 0;
				break;
			case SIZE_MAXIMIZED:
				enginePointer->test = true;
				enginePointer->HandleWindowResize(lParam & 0xFFF, (lParam >> 16) & 0xFFFF, true); return 0;
				break;
			case SIZE_RESTORED:
				if (enginePointer->test)
				{
					enginePointer->HandleWindowResize(lParam & 0xFFF, (lParam >> 16) & 0xFFFF, true);
					enginePointer->test = false;
					return 0;
				}
				enginePointer->geIsMinimized = false;
				break;
			default:
				break;
			}
			// Tell the application the window changed size
			enginePointer->HandleWindowResize(lParam & 0xFFF, (lParam >> 16) & 0xFFFF, false);

			return 0;
		}
		case WM_EXITSIZEMOVE: enginePointer->HandleWindowResize(enginePointer->geGetWindowSize().x, enginePointer->geGetWindowSize().y, true); return 0;

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