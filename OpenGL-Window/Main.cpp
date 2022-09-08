#include <windows.h>
#include <iostream>

// Engine header
//#define GAME_USE_DEDICATED_GPU
#include "Game.h"

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

		attrib.WindowTitle = "Spinning Triangle";
		attrib.GameVersion = "0.01";
		attrib.Framelock = 60;
		attrib.isVsyncOn = false;
		attrib.isDebugMode = false;
		//attrib.isWindowFullscreen = true;
		//attrib.RenderingAPI = RenderAPI::Vulkan;
		SetAttributes(attrib);
	}

	void LoadContent()
	{
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	}

	void Shutdown()
	{
	}

	void Update(const float_t msElapsed)
	{
		static double_t upsTime = 0.0f;
		static uint32_t updatesCounted = 0;

		upsTime += msElapsed;
		updatesCounted++;
		if (upsTime >= 1000.0f)
		{
			std::cout << terminal.SetPosition(0, 10) << "Updates per second : " << updatesCounted << "\n";
			
			updatesCounted = 0;
			upsTime = upsTime - 1000.0f;
		}

		// Write out mouse info if it has changed
		if ((mouse._xPositionRelative != 0) || (mouse._yPositionRelative != 0))
		{
			std::cout << terminal.SetPosition(0, 8) << terminal.EraseLine << "Mouse Relative Movement : " << mouse._xPositionRelative << "," << mouse._yPositionRelative << '\n';
			std::cout << terminal.SetPosition(0, 9) << terminal.EraseLine << "Mouse Position : " << mouse._xPosition << "," << mouse._yPosition << '\n';
		}
		// Handle Input
		if (keyboard.WasKeyReleased(VK_F11))
		{
			ToggleFullscreen();
		}
		if (keyboard.WasKeyReleased(VK_ESCAPE))
		{
			Stop();
		}
	}

	void Render(const float_t msElapsed)
	{
		static float_t fpsTime = 0.0f;
		static uint32_t framesCounted = 0;

		fpsTime += msElapsed;
		framesCounted++;
		if (fpsTime >= 1000.0f)
		{
			std::cout << terminal.SetPosition(0, 11) << "Frames per second : " << framesCounted << "\n";
			framesCounted = 0;
			fpsTime = fpsTime - 1000.0f;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glRotatef(1, 1.0, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5, 0); // Pass first vertex

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.5, 0); // Pass second vertex

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0, 0.5); // Pass third vertex

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
	engine.Start();

	return EXIT_SUCCESS;
}