#include "GameEngine.h"
#include "GameRendererGL.h"

namespace game
{
	Engine::Engine()
	{
		isRunning = false;
		enginePointer = this;
		_renderer = nullptr;

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
		_timer.Reset();
		float timeNow = 0.0f;
		do
		{
			ProcessMessages();
			timeNow = _timer.Elapsed();
			Render(timeNow);
			Update(timeNow);
			_timer.Reset();
			Swap();
		} while (isRunning);
	}

	void Engine::ProcessMessages(void)
	{
		_window.DoMessagePump();
	}

	void Engine::SetAttributes(const GameAttributes &attrib)
	{
		_attributes = attrib;
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