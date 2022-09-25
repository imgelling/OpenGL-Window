#include <windows.h>
#include <iostream>

// Engine header
#define GAME_USE_DEDICATED_GPU
#include "Game.h"

class Game : public game::Engine
{

public:
	game::Texture2dGL texture;
	game::Texture2dGL createdTexture;
	game::ShaderGL shader;
	game::Terminal terminal;
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

		if (!LoadShader("content/SpriteBatch_vert.shader","content/SpriteBatch_frag.shader", shader))
		{
			logger->Error(game::lastError);
		}
		else
		{
			logger->Write("SpriteBatch shader loaded!");
		}

		createdTexture.width = 640;
		createdTexture.height = 480;
		createdTexture.componentsPerPixel = 4;
		createdTexture.filterType = game::TextureFilterType::Point;
		if (!CreateTexture(createdTexture))
		{
			logger->Error(game::lastError);
		}
		else
		{
			logger->Write("Texture created!");
		}


		// Setup OpenGL
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_CULL_FACE);

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
		game::Vector2i windowSize;

		// bl
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
		UnLoadTexture(createdTexture);
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

		//glCallList(fullScreenTri);
		game::Vector2i windowSize = GetWindowSize();

		// scale in old lib
		float posx = 0;
		float posy = 0;
		float sx = 0;
		float sy = 0;
		float temp = 0;
		if (windowSize.height < windowSize.width)
		{

			sy = (float)windowSize.height / (float)texture.height;
			temp = (float)windowSize.width / (float)texture.width;
			if (temp > sy)
			{
				sx = sy;
			}
			else
			{
				sx = sy = temp;
				posy = ((float)windowSize.height / 2.0f - ((float)texture.height * sy / 2.0f)); // windowSize.height / 2 - (height*sy / 2);
			}
			posx = ((float)windowSize.width / 2.0f - ((float)texture.width * sx / 2.0f));

		}
		else if (windowSize.height > windowSize.width)
		{
			sx = (float)windowSize.width / (float)texture.width;
			sy = sx;
			posy = ((float)windowSize.height / 2.0f - ((float)texture.height * sy / 2.0f));
		}
		else
		{
			sx = sy = 1.0;

		}

		//if (full)
		//	sb->Draw(WindowBuffer[current], Recti(0, 0, texture.width, texture.height), Recti(0, 0, width, height), Colors::White);
		//else
		//	sb->Draw(WindowBuffer[current], Recti(posx, posy, (int)(width * sx), (int)(height * sy)), Recti(0, 0, width, height), Colors::White);

		// position.x * 2.0 / width - 1.0
		// position.y * 2.0 / height - 1.0;
		float px = posx;
		float py = posy;
		posx = ((float) posx * 2.0f / (float)windowSize.width - 1.0f);
		posy = ((float) posy * 2.0f / (float)windowSize.height - 1.0f);
		float width = (texture.width) * sx;
		width = ((float)width * 2.0f / (float)windowSize.width - 1.0f);
		float height = (texture.height) * sy;
		height = ((float)height * 2.0f / (float)windowSize.height - 1.0f);
		glBegin(GL_QUADS);
		//bl
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0);
		glVertex2f(posx, -height);
		//br
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(width, -height);
		//tr
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1);
		glVertex2f(width, -posy);
		// tl
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1);
		glVertex2f(posx, -posy);// -1.0f, 1.0f);

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