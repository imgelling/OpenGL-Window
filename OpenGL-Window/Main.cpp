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
	game::Terminal terminal; // throwing an error 6, invalid handle, doesn't show when using nvidia, now just magically gone
	uint32_t fullScreenTri;

	Game(game::Logger& logger) : game::Engine(&logger)
	{
		fullScreenTri = 0;
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
		//attrib.WindowFullscreen = true;
		//attrib.RenderingAPI = game::RenderAPI::Vulkan;
		SetAttributes(attrib);
		
	}

	void LoadContent()
	{
		if (!LoadTexture("content/Screen boundries.png", texture))
		{
			logger->Error(game::lastError);
		}
		else
		{
			logger->Write("Screen boundries.png loaded!");
		}

		if (!LoadShader("content/SpriteBatch_vert.shader","content/SpriteBatch_frag.shader", shader))
		{
			logger->Error(game::lastError);
		}
		else
		{
			logger->Write("SpriteBatch shader loaded!");
		}

		// Setup OpenGL
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);

		fullScreenTri = glGenLists(1);
		glNewList(fullScreenTri, GL_COMPILE);
		glBegin(GL_TRIANGLES);
		//// Draws a single triangle
		////bl
		//glColor3f(1.0f, 1.0f, 1.0f);
		//glTexCoord2f(0, 0);
		//glVertex2f(-1.0f, -1.0f);

		////br
		//glColor3f(1.0f, 1.0f, 1.0f);
		//glTexCoord2f(2.0f, 0.0f);
		//glVertex2f(3.0f, -1.0f);

		//// tl
		//glColor3f(1.0f, 1.0f, 1.0f);
		//glTexCoord2f(0, 2);
		//glVertex2f(-1.0f, 3.0f);


		// Fullscreen with 2 triangles
		// Bottom left triangle
		//bl
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0);
		glVertex2f(-1.0f, -1.0f);

		//br
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);

		// tl
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1);
		glVertex2f(-1.0f, 1.0f);

		// Top right triangle
		//br
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(1.0f, -1.0f);
		//tr
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1);
		glVertex2f(1.0f, 1.0f);
		// tl
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1);
		glVertex2f(-1.0f, 1.0f);

		glEnd();

		glEndList();

	}

	void Shutdown()
	{
		UnLoadTexture(texture);
		UnLoadShader(shader);
		//terminal.~Terminal();
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

		glBindTexture(GL_TEXTURE_2D, texture.bind);
		//glRotatef(0.1f, 0.0, 0.0f, 1.0f);

		glCallList(fullScreenTri);
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