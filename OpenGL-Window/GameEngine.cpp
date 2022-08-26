#include "GameEngine.h"
#include "GameRendererGL.h"

namespace game
{
	Engine::Engine()
	{
		isRunning = false;
		enginePointer = this;
		_renderer = nullptr;
		_frameTime = 0.0f;

		Initialize();
	}

	Engine::~Engine()
	{
		Shutdown();
		_renderer->DestroyDevice();
		delete _renderer;
	}

	void Engine::Start()
	{
		float msElapsed = 0.0f;
		_timer.Reset();
		_frameLockTimer.Reset();
		
		do
		{
			ProcessMessages();

			if (_frameLockTimer.Elapsed() >= _frameTime)
			{
				msElapsed = _timer.Elapsed();
				Update(msElapsed);
				Render(msElapsed);
				Swap();
				_frameLockTimer.Reset();
				_timer.Reset();
			}
		} while (isRunning);
	}

	void Engine::ProcessMessages()
	{
		_window.DoMessagePump();
	}

	void Engine::SetAttributes(const GameAttributes &attrib)
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

	void Engine::SetFrameLock(const uint32_t limit)
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
	
	void Engine::SetWindowTitle(const std::string title)
	{
		_window.SetWindowTitle(title);
	}
	bool Engine::Create()
	{
		// Set window info
		if (!_window.SetWindowInfo(_attributes.WindowTitle,
			_attributes.WindowWidth,
			_attributes.WindowHeight,
			_attributes.isWindowFullscreen,
			_attributes.isWindowBorderless))
		{
			std::cout << game::lastError;
			return false;
		}

		// Create the window
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
		else
		{
			lastError = { GameErrors::GameInvalidParameter, "Only OpenGL is implemented." };
			return false;
		}
		

		// Create rendering device
		if (!_renderer->CreateDevice(_window, _attributes.isVsync))
		{
			std::cout << game::lastError;
			_renderer->DestroyDevice();
			return false;
		}

		return true;
	}

	void Engine::Swap()
	{
		_renderer->Swap();
	}
}