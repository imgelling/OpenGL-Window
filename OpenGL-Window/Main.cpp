// Engine header
#define GAME_USE_DEDICATED_GPU
//#define GAME_SUPPORT_DIRECTX9
//#define GAME_SUPPORT_DIRECTX11
#define GAME_SUPPORT_OPENGL
//#define GAME_SUPPORT_VULKAN 
#include "Game.h"

class Game : public game::Engine
{

public:
	game::PixelMode pixelMode;
	game::SpriteBatch spriteBatch;
	game::Texture2D spriteTexture;
	game::SpriteFont spriteFont;

	Game(game::Logger& logger) : game::Engine(&logger)
	{
	}

	void Initialize()
	{
		game::Attributes attributes;
		
		attributes.WindowTitle = "PixelMode tests";
		attributes.GameVersion = "0.01";
		attributes.Framelock = 0;  
		attributes.VsyncOn = false;
		attributes.DebugMode = false;
		attributes.MultiSamples = 8; // max 8 amd, 16 nvidia
		attributes.RenderingAPI = game::RenderAPI::DirectX9; 
		//attributes.RenderingAPI = game::RenderAPI::OpenGL;
		//attributes.RenderingAPI = game::RenderAPI::DirectX11;
		
		geSetAttributes(attributes);
	}

	void LoadContent()
	{
#if defined (GAME_OPENGL)
		if (geIsUsing(GAME_OPENGL))
		{
			glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_CULL_FACE);
		}
#endif
		// Load sprite texture
		if (!geLoadTexture("Content/test.png", spriteTexture))
		{
			geLogger->Error(game::lastError);
		}

		// Setup pixel mode
		if (!pixelMode.Initialize({ 320, 240 }))
		{
			geLogger->Error(game::lastError);
		}

		// Setup sprite batch
		if (!spriteBatch.Initialize())
		{
			geLogger->Error(game::lastError);
		}

		game::Texture2D test;
		if (!geLoadTexture("Content/new.png", test))
		{
			geLogger->Error(game::lastError);
		}
		if (!spriteFont.Load("Content/new", test))
		{
			geLogger->Error(game::lastError);
		}
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
		geSetWindowTitle("fps : " + std::to_string(geGetFramesPerSecond()) + " ups : " + std::to_string(geGetUpdatesPerSecond()) + " cpu : " + std::to_string(geGetCPUFrequency()) + "Mhz");
		
		Clear();

		pixelMode.Clear(game::Colors::Blue);
		for (uint32_t i = 0; i < 320; i++)
		{
			pixelMode.PixelClip(i, 239, game::Colors::Pink);
		}
		for (uint32_t i = 0; i < 320; i++)
		{
			pixelMode.PixelClip(i, 0, game::Colors::Pink);
		}

		pixelMode.Render();

		spriteBatch.Begin();
		//for (int i = 0; i < 40; i++)
			//spriteBatch.Draw(spriteTexture, { 10 + (i * 100), 10 });

		spriteBatch.DrawString(spriteFont, "THIS IS A TEST OF THE BROADCASTING SYSTEM", 10, 200, game::Colors::Red);
		
		spriteBatch.End();

#if defined(GAME_DIRECTX9)
		if (geIsUsing(GAME_DIRECTX9))
		{
			d3d9Device->EndScene();
		}
#endif
	}

	// Clears the screen and does beginscene for dx9
	void Clear()
	{
#if defined (GAME_OPENGL)
		if (geIsUsing(GAME_OPENGL))
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
#endif

#if defined (GAME_DIRECTX9)
		if (geIsUsing(GAME_DIRECTX9))
		{
			d3d9Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(64, 64, 64), 1.0f, 0);
			d3d9Device->BeginScene();
		}
#endif

#if defined(GAME_DIRECTX11)
		if (geIsUsing(GAME_DIRECTX11))
		{
			float color[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
			d3d11Context->ClearRenderTargetView(d3d11RenderTarget, color);
		}
#endif
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