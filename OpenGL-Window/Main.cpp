// Engine header
#define GAME_USE_DEDICATED_GPU
//#define GAME_USE_SHADERS
#define GAME_SUPPORT_DIRECTX9
//#define GAME_SUPPORT_DIRECTX11
//#define GAME_SUPPORT_DIRECTX12
//#define GAME_SUPPORT_OPENGL
//#define GAME_SUPPORT_VULKAN 
//#define GAME_SUPPORT_ALL
#include "Game.h"

constexpr uint32_t MAX_UPDATES = 0;
constexpr uint32_t MIN_UPDATES = 10;
constexpr uint32_t MAX_FRAMES = 0;
constexpr uint32_t MIN_FRAMES = 10;


//
// Need for dx9
// internal pixel type  -- not sure how to do
//

class Game : public game::Engine
{

public:
	game::PixelMode pixelMode;
	game::SpriteBatch spriteBatch;
	game::Texture2D spriteTexture;
	game::SpriteFont spriteFont;
	game::GameRandom random;
	game::PerformanceTimer perftimer;

	Game(game::Logger& logger) : game::Engine(&logger)
	{
	}

	void HandleWindowSizeChange() override
	{
		if (geIsMinimized)
		{
			geSetGameLocks(MIN_FRAMES, MIN_UPDATES);
		}
		else
		{
			geSetGameLocks(MAX_FRAMES, MAX_UPDATES);
		}
	}

	void Initialize()
	{
		game::Attributes attributes;
		
		attributes.WindowTitle = "PixelMode tests";
		attributes.GameVersion = "0.01";
		attributes.FrameLock = MAX_FRAMES;
		attributes.UpdateLock = MAX_UPDATES;
		attributes.VsyncOn = false;
		attributes.DebugMode = false;
		attributes.MultiSamples = 8;
		attributes.RenderingAPI = game::RenderAPI::DirectX9; 
		//attributes.RenderingAPI = game::RenderAPI::OpenGL;
		//attributes.RenderingAPI = game::RenderAPI::DirectX11;
		//attributes.RenderingAPI = game::RenderAPI::DirectX12;
		
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

		// Load font for output to screen
		if (!spriteFont.Load("Content/new.fnt", "Content/new.png"))
		{
			geLogLastError();
		}		

		//D3DCompileFromFile()
	}

	void Shutdown()
	{
	}

	void Update(const float_t msElapsed)
	{
		//geSetWindowTitle(std::to_string(geGetUpdatesPerSecond()) + " fps = " + std::to_string(geGetFramesPerSecond()));
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

	void DrawHorizontalPill(const uint32_t x, const uint32_t y, const uint32_t length, const uint32_t radius, const game::Color& color)
	{
		uint32_t calculatedLength = length - radius - radius;

		//if (radius < 0 || x < -radius || y < -radius || x - _bufferSize.width > radius || y - _bufferSize.height > radius)
			//return;

		if (radius > 0)
		{
			int32_t x0 = 0;
			int32_t y0 = radius;
			int32_t d = 3 - 2 * radius;

			auto drawline = [&](int32_t sx, int32_t ex, int32_t y)
			{
				//for (int32_t x = sx; x <= ex; x++)
				//	pixelMode.Pixel(x, y, color);
				pixelMode.LineClip(sx, y, ex + calculatedLength, y, color);
			};

			while (y0 >= x0)
			{
				drawline(x - y0, x + y0, y - x0);
				if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						drawline(x - x0, x + x0, y - y0);
						drawline(x - x0, x + x0, y + y0);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			pixelMode.Pixel(x, y, color);
	}

	void Render(const float_t msElapsed)
	{
		game::Pointi scaledMousePos;

		scaledMousePos = pixelMode.GetScaledMousePosition();

		// Clears and starts new scene
		BeginScene();

		pixelMode.Clear(game::Colors::Blue);

		// Top and bottom
		pixelMode.LineClip(340, 0, -1, 0, game::Colors::Pink);
		pixelMode.LineClip(-10, 239, 409, 239, game::Colors::Pink);

		// Left and right
		pixelMode.LineClip(0, -10, 0, 300, game::Colors::Pink);
		pixelMode.LineClip(319, 500, 319, -10, game::Colors::Pink);

		//int32_t rx1 = random.RndRange(0, 340);
		//int32_t ry1 = random.RndRange(0, 340);

		////int32_t rx2 = random.RndRange(0, 340);
		////int32_t ry2 = random.RndRange(0, 340);
		//game::Color rndColor;
		//perftimer.Start("CircleClip");
		//for (uint32_t count = 0; count < 500000; count++)
		//{
		//	//rndColor.Set(random.RndRange(0, 255), random.RndRange(0, 255), random.RndRange(0, 255), 255);
		//	pixelMode.PixelClip(rx1 - 10, ry1 - 10, game::Colors::White);
		//	rx1 = random.RndRange(0, 340);
		//	ry1 = random.RndRange(0, 340);
		//	//rx2 = random.RndRange(0, 340);
		//	//ry2 = random.RndRange(0, 340);
		//}
		//perftimer.Stop("CircleClip");

		// Circle Clipped
		//pixelMode.CircleFilledClip(scaledMousePos.x, scaledMousePos.y, 40, game::Colors::Green);
		DrawHorizontalPill(scaledMousePos.x, scaledMousePos.y, 40, 5, game::Colors::Green);
		DrawHorizontalPill(scaledMousePos.x, scaledMousePos.y, 38, 4, game::Colors::White);

		// Weird diagonal
		pixelMode.LineClip(-20, -10, scaledMousePos.x, scaledMousePos.y, game::Colors::Pink);

		// Rectangle
		game::Recti rect;
		rect.left = 1;
		rect.top = 1;
		rect.right = 318;
		rect.bottom = 238;
		pixelMode.Rect(rect, game::Colors::White);


		// Circle
		pixelMode.CircleFilled(160, 120, 75, game::Colors::DarkGray);

		pixelMode.Render();

		spriteBatch.Begin();
		for (int i = 0; i < 40; i++)
			spriteBatch.Draw(spriteTexture, { 10 + (i * 100), 10 }, game::Colors::White);
		//double_t perSecond = (perftimer.LastRun("CircleClip") / 1000.0 / 1000.0 / 1000.0) * 500000.0;
		//spriteBatch.DrawString(spriteFont, "Random PixelClip(s) : " + std::to_string(perSecond) + " billion per second.", 10, 200, game::Colors::Red);
		spriteBatch.DrawString(spriteFont, "FPS : " + std::to_string(geGetFramesPerSecond()) + " UPS : " + std::to_string(geGetUpdatesPerSecond()) + " cpu : " + std::to_string(geGetCPUFrequency()) + "Mhz", 10, 200, game::Colors::Red);
		spriteBatch.DrawString(spriteFont, "Window Pixel Size: " + std::to_string(geGetWindowSize().width) + "x" + std::to_string(geGetWindowSize().height), 10, 220, game::Colors::Red);
		spriteBatch.DrawString(spriteFont, "PixelMode Pixel Size: " + std::to_string(pixelMode.GetPixelFrameBufferSize().width) + "x" + std::to_string(pixelMode.GetPixelFrameBufferSize().height), 10, 240, game::Colors::Red);
		spriteBatch.End();

		EndScene();
	}

	// Clears the screen
	void BeginScene()
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
			d3d9Device->BeginScene();
			d3d9Device->Clear(0, NULL, D3DCLEAR_TARGET, game::Colors::DarkGray.packedARGB, 1.0f, 0);
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

	// Ends the scene
	void EndScene()
	{
#if defined (GAME_DIRECTX9)
		if (geIsUsing(GAME_DIRECTX9))
		{
			d3d9Device->EndScene();
		}
#endif
	}

	// Sets up API stuff
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