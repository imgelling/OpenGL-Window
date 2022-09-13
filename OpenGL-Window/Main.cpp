#include <windows.h>
#include <iostream>



// Engine header
//#define GAME_USE_DEDICATED_GPU
#include "Game.h"

class Game;
uint32_t bindTexture;
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
		attrib.Framelock = 0;
		attrib.isVsyncOn = true;
		attrib.isDebugMode = true;
		//attrib.isWindowFullscreen = true;
		//attrib.RenderingAPI = RenderAPI::Vulkan;
		SetAttributes(attrib);
	}

	void LoadContent()
	{
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		if (!LoadTexture("content/test.png"))
		{
			logger->Error(game::lastError);
		}
		glEnable(GL_TEXTURE_2D);
	}

	void Shutdown()
	{
	}

	void Update(const float_t msElapsed)
	{
		static double_t upsTime = 0.0f;

		upsTime += msElapsed;
		if (upsTime >= 1000.0f)
		{
			std::cout << terminal.SetPosition(0, 10) << "Updates per second : " << GetUpdatesPerSecond() << "\n";
			upsTime = upsTime - 1000.0f;
		}
		
		//// Write out mouse info if it has changed
		//if (mouse.GetWheelDelta())
		//{
		//	std::cout << terminal.SetPosition(0, 7) << terminal.EraseLine << "Mouse Wheel Delta : " << mouse.GetWheelDelta() << "\n";
		//}
		//if ((mouse.GetPositionRelative().x != 0) || (mouse.GetPositionRelative().y != 0))
		//{
		//	std::cout << terminal.SetPosition(0, 8) << terminal.EraseLine << "Mouse Relative Movement : " << mouse.GetPositionRelative().x << "," << mouse.GetPositionRelative().y << '\n';
		//	std::cout << terminal.SetPosition(0, 9) << terminal.EraseLine << "Mouse Position : " << mouse.GetPosition().x << "," << mouse.GetPosition().y << '\n';
		//}

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
		static float_t fpsTime = 0.0f;

		fpsTime += msElapsed;
		if (fpsTime >= 1000.0f)
		{
			std::cout << terminal.SetPosition(0, 11) << "Frames per second : " << GetFramesPerSecond() << "\n";
			fpsTime = fpsTime - 1000.0f;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, bindTexture);
		glRotatef(1, 1.0, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);

		//glColor3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-0.5, 0); // Pass first vertex

		//glColor3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(0.5, 0); // Pass second vertex

		//glColor3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
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
	engine.StartEngine();

	return EXIT_SUCCESS;
}