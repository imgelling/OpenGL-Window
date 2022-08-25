#include "GameEngine.h"
#include "GameRendererGL.h"

namespace game
{
	Engine::Engine()
	{
		isRunning = false;
		enginePointer = this;
		renderer = nullptr;
	}

	Engine::~Engine()
	{
		renderer->DestroyDevice();
		delete renderer;
	}

	void Engine::ProcessMessages(void)
	{
		window.DoMessagePump();
	}

	void Engine::SetAttributes(const GameAttributes &attrib)
	{
		_attributes = attrib;
	}

	bool Engine::Start()
	{
		// Set window info
		if (!window.SetWindowInfo(_attributes.WindowTitle,
			_attributes.WindowWidth,
			_attributes.WindowHeight,
			_attributes.WindowFullscreen,
			_attributes.WindowBorderless))
		{
			std::cout << game::lastError;
			return false;
		}

		// Create the window
		if (!window.CreateTheWindow())
		{
			std::cout << game::lastError;
			return false;
		}

		// Set the renderer
		if (_attributes.RenderingAPI == RenderAPI::OpenGL)
		{
			renderer = new game::RendererGL();
		}
		else
		{
			lastError = { GameErrors::GameInvalidParameter, "Only OpenGL is implemented." };
			return false;
		}
		

		// Create rendering device
		if (!renderer->CreateDevice(window, true))
		{
			std::cout << game::lastError;
			renderer->DestroyDevice();
			return false;
		}

		return true;
	}

	void Engine::Swap()
	{
		renderer->Swap();
	}
}