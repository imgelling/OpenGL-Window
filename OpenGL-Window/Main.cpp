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
	game::Color whitecol = { 1.0f, 1.0f, 1.0f, 1.0f };
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

		if (!LoadShader("content/SpriteBatch_vert.shader", "content/SpriteBatch_frag.shader", shader))
		{
			logger->Error(game::lastError);
		}
		else
		{
			logger->Write("SpriteBatch shader loaded!");
		}
		std::cout << whitecol.r << whitecol.g << "\n";

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
		glEnable(GL_CULL_FACE);

		fullScreenTri = glGenLists(1);
		glNewList(fullScreenTri, GL_COMPILE);
		{
			glBegin(GL_TRIANGLES);
			{

			}
			glEnd();
		}
		glEndList();
	}

	void Shutdown()
	{
		UnLoadTexture(texture);
		UnLoadTexture(createdTexture);
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

		glBindTexture(GL_TEXTURE_2D, texture.bind);
		//glRotatef(0.1f, 0.0, 0.0f, 1.0f);

		//glCallList(fullScreenTri);

		// ---------- Begin scaling of quad
		game::Vector2i windowSize = GetWindowSize();

		// scale in old lib
		game::Vector2f positionOfScaledTexture;
		game::Vector2f scale;
		game::Vector2f sizeOfScaledTexture;
		float_t tempScale = 0.0f;


		if (windowSize.height < windowSize.width)
		{
			scale.y = (float_t)windowSize.height * texture.oneOverHeight;
			tempScale = (float_t)windowSize.width * texture.oneOverWidth;
			if (tempScale > scale.y)
			{
				scale.x = scale.y;
			}
			else
			{
				scale.x = scale.y = tempScale;
				positionOfScaledTexture.y = (((float_t)windowSize.height / 2.0f) - ((float_t)texture.height * scale.y / 2.0f));
			}
			positionOfScaledTexture.x = (((float_t)windowSize.width / 2.0f) - ((float_t)texture.width * scale.x / 2.0f));
		}
		else if (windowSize.height > windowSize.width)
		{
			scale.x = (float_t)windowSize.width * texture.oneOverWidth;
			scale.y = scale.x;
			positionOfScaledTexture.y = (((float_t)windowSize.height / 2.0f) - ((float_t)texture.height * scale.y / 2.0f));
		}
		else
		{
			scale = { 1.0f, 1.0f };
		}

		// Set the size of the scaled texture
		sizeOfScaledTexture.width = positionOfScaledTexture.x + (texture.width * scale.x);
		sizeOfScaledTexture.height = positionOfScaledTexture.y + (texture.height * scale.y);

		// Homoginize the scaled rect to -1 to 1 range using
		// position.x = position.x * 2.0 / width - 1.0
		// position.y = position.y * 2.0 / height - 1.0;
		positionOfScaledTexture.x = (positionOfScaledTexture.x * 2.0f / (float_t)windowSize.width) - 1.0f;
		positionOfScaledTexture.y = (positionOfScaledTexture.y * 2.0f / (float_t)windowSize.height) - 1.0f;
		sizeOfScaledTexture.width = ((float_t)sizeOfScaledTexture.width * 2.0f / (float_t)windowSize.width) - 1.0f;
		sizeOfScaledTexture.height = ((float_t)sizeOfScaledTexture.height * 2.0f / (float_t)windowSize.height) - 1.0f;

		// ---------- End scaling of quad


		// Draw the quad needs to be a drawlist that only updates on window size change
		glBegin(GL_QUADS);
		//bl
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0);
		glVertex2f(positionOfScaledTexture.x, -sizeOfScaledTexture.height);
		//br
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(sizeOfScaledTexture.width, -sizeOfScaledTexture.height);
		//tr
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1);
		glVertex2f(sizeOfScaledTexture.width, -positionOfScaledTexture.y);
		// tl
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1);
		glVertex2f(positionOfScaledTexture.x, -positionOfScaledTexture.y);

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