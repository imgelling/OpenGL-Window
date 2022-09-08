#include "GameEngine.h"
#include "GameRendererGL.h"
#include "GameRendererVK.h"



namespace game
{

	Engine::Engine(Logger* logger)
	{
		isRunning = false;
		enginePointer = this;
		_renderer = nullptr;
		_frameTime = 0.0f;
		this->logger = logger;
	}

	Engine::~Engine()
	{
		if (_renderer) _renderer->DestroyDevice();
		delete _renderer;
	}

	void Engine::Start()
	{
		// Storage of time
		float_t msElapsed = 0.0f;

		_renderTimer.Reset();
		_frameLockTimer.Reset();
		_updateTimer.Reset();
	
		do
		{
			// Do window messages
			_ProcessMessages();

			msElapsed = _updateTimer.Elapsed();
			if (msElapsed > 0.0f)
			{
				Update(msElapsed);
				_updateTimer.Reset();
			}

			if (_frameLockTimer.Elapsed() >= _frameTime)
			{
				_frameLockTimer.Reset();
				msElapsed = _renderTimer.Elapsed();
				_renderTimer.Reset();
				Render(msElapsed);
				_Swap();
			}

		} while (isRunning);

		Shutdown();
	}

	void Engine::Stop()
	{
		isRunning = false;
	}

	void Engine::_ProcessMessages()
	{
		_window.DoMessagePump();
	}

	void Engine::SetAttributes(const Attributes &attrib)
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
		_attributes.WindowTitle = title;
	}

	void Engine::ToggleFullscreen()
	{
		_window.ToggleFullScreen();
		_attributes.isWindowFullscreen = !_attributes.isWindowFullscreen;
	}
	
	bool Engine::Create()
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
		_renderer->SetAttributes(_attributes);

		// Create rendering device
		if (!_renderer->CreateDevice(_window))
		{
			_renderer->DestroyDevice();
			return false;
		}

		// Load user content
		LoadContent();

		return true;
	}

	void Engine::_Swap()
	{
		_renderer->Swap();
	}

	void Engine::HandleWindowResize(const uint32_t width, const uint32_t height)
	{
		if (_renderer)
		{
			_renderer->HandleWindowResize(width, height);
		}
	}
}