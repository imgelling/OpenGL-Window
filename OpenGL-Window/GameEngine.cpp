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
		static double frameTimeAccum = 0.0f;
		double msElapsed = 0.0f;

		_timer.Reset();
		_frameLockTimer.Reset();
		
		do
		{
			ProcessMessages();
			
			if (_frameLockTimer.Elapsed() >= _frameTime)
			{
				_frameLockTimer.Reset();
				Update(_timer.Elapsed());
				Render(_timer.Elapsed());
				_timer.Reset();
				Swap();
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
			_frameTime = 1000.0 / (double)_attributes.Framelock;
		}
		else
		{
			_frameTime = 0;
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