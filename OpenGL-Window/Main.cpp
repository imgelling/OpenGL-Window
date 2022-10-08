
#include <windows.h>
#include <iostream>

// Engine header
#define GAME_USE_DEDICATED_GPU
#include "Game.h"

class Game : public game::Engine
{

public:
	game::Terminal terminal; // error 6 randomly
	game::Color whitecol = { 1.0f, 1.0f, 1.0f, 1.0f };
	game::PixelModeFixed pixelMode;

	Game(game::Logger& logger) : game::Engine(&logger)
	{
	}

	void Initialize()
	{
		game::Attributes attrib;

		attrib.WindowTitle = "PixelMode tests";
		attrib.GameVersion = "0.01";
		attrib.Framelock = 60;
		attrib.VsyncOn = false;
		attrib.DebugMode = true;
		attrib.MultiSamples = 32; // max 8 amd, 16 nvidia
		attrib.RenderingAPI = game::RenderAPI::OpenGL;
		SetAttributes(attrib);
	}

	void LoadContent()
	{
		whitecol.Set(1.0f, 0.0f, 1.0f, 0.25f);

		// Setup OpenGL
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);

		// Setup pixel mode
		if (!pixelMode.Initialize({ 320, 240 }))
		{
			logger->Error(game::lastError);
		}
	}

	void Shutdown()
	{
	}

	void Update(const float_t msElapsed)
	{
		// Handle Input
		if (keyboard.WasKeyReleased(VK_F11))
		{
			ToggleFullscreen();
		}
		if (keyboard.WasKeyReleased(VK_ESCAPE))
		{
			StopEngine();
		}
	}

	void Render(const float_t msElapsed)
	{
		SetWindowTitle("fps : " + std::to_string(GetFramesPerSecond()) + " ups : " + std::to_string(GetUpdatesPerSecond()));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
															
		pixelMode.Clear(game::Colors::Black);
		for (uint32_t i = 0; i < 256; i++)
		{
			pixelMode.PixelClip(i, 10, { 1.0f, 0.0f, 1.0f, 1.0f});
		}

		pixelMode.Render();
		glFlush();
	}
};

int main()
{
	game::Logger logger("Log.html");
	Game engine(logger);

	// Create the needed bits for the engine
	if (!engine.Create())
	{
		logger.Error(game::lastError);
		return EXIT_FAILURE;
	}

	// Start the engine
	engine.StartEngine();

	return EXIT_SUCCESS;
}