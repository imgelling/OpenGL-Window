#include <windows.h>
#include <iostream>

// Engine header
//#define GAME_USE_DEDICATED_GPU
#include "Game.h"

uint32_t bindTexture; // hacky, needs better way Texture Class maybe


class Game : public game::Engine
{

public:
	game::Terminal terminal;

	Game(game::Logger& logger) : game::Engine(&logger)
	{
	}

	void Initialize()
	{
		game::Attributes attrib;

		attrib.WindowTitle = "Textured Spinning Quad";
		attrib.GameVersion = "0.01";
		attrib.Framelock = 60;
		attrib.VsyncOn = false;
		attrib.DebugMode = true;
		attrib.MultiSamples = 32; // max 8 amd, 16 nvidia
		//attrib.WindowFullscreen = true;
		//attrib.RenderingAPI = game::RenderAPI::Vulkan;
		SetAttributes(attrib);
	}

	void LoadContent()
	{
		if (!LoadTexture("content/test.png"))
		{
			logger->Error(game::lastError);
		}
		else
		{
			logger->Write("test.png loaded!");
		}


		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_CULL_FACE);
	}

	void Shutdown()
	{
		glDeleteTextures(1, &bindTexture);
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

		glBindTexture(GL_TEXTURE_2D, bindTexture);
		glRotatef(1.0f, 0.0, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);

		// TL triangle
		glColor3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-0.5, 0.5f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-0.5, -0.5);

		glColor3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(0.5, 0.5);

		// BR triangle
		glColor3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(0.5, 0.5); 

		glColor3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-0.5, -0.5); 

		glColor3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(0.5, -0.5);


		glEnd();
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