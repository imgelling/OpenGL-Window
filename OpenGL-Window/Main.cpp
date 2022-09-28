#include <windows.h>
#include <iostream>

// Engine header
//#define GAME_USE_DEDICATED_GPU
#include "Game.h"

class Game : public game::Engine
{

public:
	game::Texture2dGL texture;
	game::ShaderGL shader;
	game::Terminal terminal; // error 6 when clicking x button to close
	game::Color whitecol = { 1.0f, 1.0f, 1.0f, 1.0f };
	game::PixelMode pixelMode;
	
	// pixel mode stuff
	//game::Texture2dGL createdTexture;
	uint32_t fullScreenTri;
	uint32_t* video = nullptr;
	game::Vector2i bufferSize = { 320,240 };


	Game(game::Logger& logger) : game::Engine(&logger)
	{
		fullScreenTri = 0;

		// pixel mode stuff
		video = new uint32_t[bufferSize.width*bufferSize.height];
		if (video == nullptr)
		{
			//error out
		}
	}

	void Initialize()
	{
		game::Attributes attrib;

		attrib.WindowTitle = "Full Screen single triangle";
		attrib.GameVersion = "0.01";
		attrib.Framelock = 60;
		attrib.VsyncOn = false;
		attrib.DebugMode = true;
		attrib.MultiSamples = 32; // max 8 amd, 16 nvidia
		SetAttributes(attrib);
	}

	void LoadContent()
	{
		texture.filterType = game::TextureFilterType::Point;
		if (!LoadTexture("content/Screen boundries.png", texture))
		{
			logger->Error(game::lastError);
		}
		else
		{
			logger->Write("Screen boundries.png loaded!");
		}

		if (!LoadShader("content/SpriteBatch_vert.shader", "content/SpriteBatch_frag.shader", shader))
		{
			logger->Error(game::lastError);
		}
		else
		{
			logger->Write("SpriteBatch shader loaded!");
		}
		whitecol.Set(1.0f, 0.0f, 1.0f, 1.0f);

		// Setup OpenGL
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		// pixel mode needs blend off
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);


		if (!pixelMode.Initialize({ 320, 240 }))
		{
			logger->Error(game::lastError);
		}
	}

	void Shutdown()
	{
		UnLoadTexture(texture);
		UnLoadShader(shader);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// pixel mode stuff
		pixelMode.Clear(whitecol);
		for (int i = 10; i < 100; i++)
			pixelMode.Pixel(i, 10, { 1.0f, 1.0f, 1.0f, 1.0f });

		pixelMode.Render();
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