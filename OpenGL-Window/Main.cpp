
#include <windows.h>
#include <iostream>


// Engine header
//#define GAME_USE_DEDICATED_GPU
#define GAME_SUPPORT_DIRECTX9
#define GAME_SUPPORT_OPENGL
#include "Game.h"

class Game : public game::Engine
{

public:
	game::Terminal terminal;
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
		attrib.MultiSamples = 8; // max 8 amd, 16 nvidia
		attrib.RenderingAPI = game::RenderAPI::DirectX9;// :OpenGL;
		geSetAttributes(attrib);
	}

	void LoadContent()
	{

		geSetClearColor(game::Colors::DarkGray);

		geEnable(GAME_BLEND);
		//geEnable(GAME_CULL_FACE); 

		// Setup pixel mode
		if (!pixelMode.Initialize({ 320, 240 }))
		{
			geLogger->Error(game::lastError);
		}

		//game::Texture2dGL test;
		//if (!geLoadTexture("content/test.png", test))
		//{
		//	std::cout << "---------------- faile\n";
		//}
	}

	void Shutdown()
	{
	}

	void Update(const float_t msElapsed)
	{
		// Handle Input
		if (geKeyboard.WasKeyReleased(VK_F11))
		{
			geToggleFullscreen();
		}
		if (geKeyboard.WasKeyReleased(VK_ESCAPE))
		{
			geStopEngine();
		}
	}

	void Render(const float_t msElapsed)
	{
		static bool first = true;
		if (first)
		{
			first = false;
			return;
		}
		geSetWindowTitle("fps : " + std::to_string(geGetFramesPerSecond()) + " ups : " + std::to_string(geGetUpdatesPerSecond()) + " cpu : " + std::to_string(geGetCPUFrequency()) + "Mhz");
		
		geClear(true, true, false);
		pixelMode.Clear(game::Colors::Black);
		for (uint32_t i = 0; i < 256; i++)
		{
			pixelMode.PixelClip(i, 10, { 1.0f, 0.0f, 1.0f, 1.0f});
		}

		pixelMode.Render();
	}
};

int main()
{
	game::Logger logger("Log.html");
	Game engine(logger);

	// Create the needed bits for the engine
	if (!engine.geCreate())
	{
		logger.Error(game::lastError);
		return EXIT_FAILURE;
	}

	// Start the engine
	engine.geStartEngine();

	return EXIT_SUCCESS;
}