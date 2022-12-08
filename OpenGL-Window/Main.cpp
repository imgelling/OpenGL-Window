// Engine header
#define GAME_USE_DEDICATED_GPU
//#define GAME_USE_SHADERS
//#define GAME_SUPPORT_DIRECTX9
//#define GAME_SUPPORT_DIRECTX11
//#define GAME_SUPPORT_OPENGL
//#define GAME_SUPPORT_VULKAN 

// need to force GAME_USE_SHADERS with dx12/vulkan and maybe dx11 maybe
#define GAME_SUPPORT_ALL
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
		attributes.DebugMode = true;
		attributes.MultiSamples = 8;
		attributes.RenderingAPI = game::RenderAPI::DirectX9; 
		//attributes.RenderingAPI = game::RenderAPI::OpenGL;
		//attributes.RenderingAPI = game::RenderAPI::DirectX11;
		
		geSetAttributes(attributes);
	}

	void LoadContent()
	{
		// Setup renderer(s)
		Setup();

		// Load sprite texture
		if (!geLoadTexture("Content/test.png", spriteTexture))
		{
			geLogLastError();
		}

		// Setup pixel mode
		if (!pixelMode.Initialize({ 320, 240 }))
		{
			geLogLastError();
		}

		// Setup sprite batch
		if (!spriteBatch.Initialize())
		{
			geLogLastError();
		}

		if (!spriteFont.Load("Content/new.fnt", "Content/new.png"))
		{
			geLogLastError();
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
		// Api dependent methods in this
		Clear();

#if defined (GAME_DIRECTX9)
		if (geIsUsing(GAME_DIRECTX9))
		{
			d3d9Device->BeginScene();
		}
#endif

		pixelMode.Clear(game::Colors::Blue);

		// Top and bottom
		pixelMode.LineClip(340, 0, -1, 0, game::Colors::Pink);
		pixelMode.LineClip(-10, 239, 409, 239, game::Colors::Pink);

		// Left and right
		pixelMode.LineClip(0, -10, 0, 300, game::Colors::Pink);
		pixelMode.LineClip(319, 500, 319, -10, game::Colors::Pink);

		// Weird diagonal
		pixelMode.LineClip(30, -20, 150, 300, game::Colors::Pink);

		// Rectangle
		game::Recti rect;
		rect.left = 1;
		rect.top = 1;
		rect.right = 318;
		rect.bottom = 238;
		pixelMode.Rect(rect, game::Colors::White);

		// Rectangle Clipped
		rect.left = -20;
		rect.top = -20;
		rect.right = 60;
		rect.bottom = 60;
		pixelMode.RectClip(rect, game::Colors::Green);

		pixelMode.Render();

		spriteBatch.Begin();
		for (int i = 0; i < 40; i++)
			spriteBatch.Draw(spriteTexture, { 10 + (i * 100), 10 }, game::Colors::White);

		spriteBatch.DrawString(spriteFont, "fps : " + std::to_string(geGetFramesPerSecond()) + " ups : " + std::to_string(geGetUpdatesPerSecond()) + " cpu : " + std::to_string(geGetCPUFrequency()) + "Mhz", 10, 200, game::Colors::Red);
		spriteBatch.DrawString(spriteFont, "Window Size: " + std::to_string(geGetWindowSize().width) + "x" + std::to_string(geGetWindowSize().height), 10, 220, game::Colors::Red);
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

	void Setup()
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
	}
};

int main()
{
	game::Logger logger("Log.html");
	Game engine(logger);

	// Create the needed bits for the engine
	if (!engine.geCreate())
	{
		engine.geLogLastError();
		return EXIT_FAILURE;
	}

	// Start the engine
	engine.geStartEngine();

	return EXIT_SUCCESS;
}