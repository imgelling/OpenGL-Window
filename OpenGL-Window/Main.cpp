// Engine header
#define GAME_USE_DEDICATED_GPU
//#define GAME_ENABLE_NETWORKING
//#define GAME_ENABLE_SHADERS
//#define GAME_SUPPORT_DIRECTX9
//#define GAME_SUPPORT_DIRECTX10
//#define GAME_SUPPORT_DIRECTX11
#define GAME_SUPPORT_DIRECTX12
//#define GAME_SUPPORT_OPENGL
//#define GAME_SUPPORT_VULKAN 
//#define GAME_SUPPORT_ALL
#include "Game.h"

constexpr uint32_t MAX_UPDATES = 0;
constexpr uint32_t MIN_UPDATES = 10;
constexpr uint32_t MAX_FRAMES = 0;
constexpr uint32_t MIN_FRAMES = 10;

class Game : public game::Engine
{

public:
	game::PixelMode pixelMode;
	game::SpriteBatch spriteBatch;
	game::Texture2D spriteTexture;
	game::SpriteFont spriteFont;
	game::Random random;
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

		// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(613);
#endif
		
		attributes.WindowTitle = "PixelMode tests";
		attributes.GameVersion = "0.01";
		attributes.FrameLock = MAX_FRAMES;
		attributes.UpdateLock = MAX_UPDATES;
		attributes.VsyncOn = false;
		attributes.DebugMode = true;
		attributes.MultiSamples = 8;
		//attributes.RenderingAPI = game::RenderAPI::DirectX9; 
		attributes.RenderingAPI = game::RenderAPI::DirectX10;
		attributes.RenderingAPI = game::RenderAPI::DirectX11;
		attributes.RenderingAPI = game::RenderAPI::DirectX12;
		//attributes.RenderingAPI = game::RenderAPI::OpenGL;
		
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

		// Temp load shader
		game::Shader shader;
#if defined(GAME_DIRECTX9) || defined(GAME_DIRECTX10)
		//shader.isPrecompiled = true;
		//if (geIsUsing(GAME_DIRECTX9) || geIsUsing(GAME_DIRECTX10))
		//{
		//	if (!geLoadShader("Content/VertexShader.cso", "Content/PixelShader.cso", shader))
		//	{
		//		geLogLastError();
		//	}
		//	geUnLoadShader(shader);
		//}
#endif

		// Setup pixel mode
		if (!pixelMode.Initialize({ 320, 240 }))
		{
			//std::cout << "failt!\n";
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
	}

	void Shutdown()
	{
		geUnLoadTexture(spriteTexture);
	}

	void Update(const float_t msElapsed)
	{
		// Handle Input
		if (geKeyboard.WasKeyReleased(geK_F11))
		{
			geToggleFullscreen();
		}
		if (geKeyboard.WasKeyReleased(geK_ESCAPE))
		{
			geStopEngine();
		}
	}
		
	void Render(const float_t msElapsed)
	{
		game::Pointi scaledMousePos;

		scaledMousePos = pixelMode.GetScaledMousePosition();

		// Clears and starts new scene
		BeginScene();

		//pixelMode.Clear(game::Colors::Blue);

		//// Top and bottom
		//pixelMode.LineClip(340, 0, -1, 0, game::Colors::Pink);
		//pixelMode.LineClip(-10, 239, 409, 239, game::Colors::Pink);

		//// Left and right
		//pixelMode.LineClip(0, -10, 0, 300, game::Colors::Pink);
		//pixelMode.LineClip(319, 500, 319, -10, game::Colors::Pink);

		////int32_t rx1 = random.RndRange(0, 340);
		////int32_t ry1 = random.RndRange(0, 340);

		////int32_t rx2 = random.RndRange(11, 329);
		////int32_t ry2 = random.RndRange(11, 329);
		////game::Color rndColor;
		////perftimer.Start("CircleClip");
		////for (uint32_t count = 0; count < 500000; count++)
		////{
		////	rx1 = random.RndRange(10, 310);;
		////	ry1 = random.RndRange(10, 240-11);;
		////	rndColor.Set(random.RndRange(0, 255), random.RndRange(0, 255), random.RndRange(0, 255), 255);
		////	pixelMode.CircleClip(rx1, ry1, 10, rndColor);
		////	//rx2 = random.RndRange(11, 329);;
		////	//ry2 = random.RndRange(11, 329);;
		////}
		////perftimer.Stop("CircleClip");

		//pixelMode.VerticalPillClip(scaledMousePos.x, scaledMousePos.y, 40, 5, game::Colors::Green);
		//pixelMode.VerticalPillClip(scaledMousePos.x, scaledMousePos.y, 38, 4, game::Colors::White);

		//// Weird diagonal
		//pixelMode.LineClip(-20, -10, scaledMousePos.x, scaledMousePos.y, game::Colors::Pink);

		//// Rectangle
		//game::Recti rect;
		//rect.left = 1;
		//rect.top = 1;
		//rect.right = 318;
		//rect.bottom = 238;
		//pixelMode.Rect(rect, game::Colors::White);

		//pixelMode.Render();

		//spriteBatch.Begin();
		////double_t perSecond = 500000.0 / (perftimer.LastRun("CircleClip") / 1000000000.0);  // throws if not found
		////double_t millionPerSecond = perSecond / 1000.0 / 1000.0;
		//for (int i = 0; i < 40; i++)
		//	spriteBatch.Draw(spriteTexture, { 10 + (i * 100), 10 }, game::Colors::White);
		////spriteBatch.DrawString(spriteFont, "Random Circle(s) : " + std::to_string(millionPerSecond) + " million per second.", 10, 0, game::Colors::Red);
		//spriteBatch.DrawString(spriteFont, "FPS : " + std::to_string(geGetFramesPerSecond()) + " UPS : " + std::to_string(geGetUpdatesPerSecond()) + " cpu : " + std::to_string(geGetCPUFrequency()) + "Mhz", 10, 200, game::Colors::Red);
		//spriteBatch.DrawString(spriteFont, "Window Pixel Size: " + std::to_string(geGetWindowSize().width) + "x" + std::to_string(geGetWindowSize().height), 10, 220, game::Colors::Red);
		//spriteBatch.DrawString(spriteFont, "PixelMode Pixel Size: " + std::to_string(pixelMode.GetPixelFrameBufferSize().width) + "x" + std::to_string(pixelMode.GetPixelFrameBufferSize().height), 10, 240, game::Colors::Red);
		//spriteBatch.End();

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
#if defined (GAME_DIRECTX10)
		if (geIsUsing(GAME_DIRECTX10))
		{
			
			d3d10Device->ClearRenderTargetView(d3d10RenderTargetView, game::Colors::DarkGray.rgba);
			d3d10Device->ClearDepthStencilView(d3d10DepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
		}
#endif

#if defined(GAME_DIRECTX11)
		if (geIsUsing(GAME_DIRECTX11))
		{
			float color[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
			d3d11DeviceContext->ClearRenderTargetView(d3d11RenderTarget, game::Colors::DarkGray.rgba);
			d3d11DeviceContext->ClearDepthStencilView(d3d11DepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
		}
#endif
#if defined(GAME_DIRECTX12)
		if (geIsUsing(GAME_DIRECTX12))
		{
			game::RendererDX12* temp = geGetRenderer();
			temp->StartFrame();
			//temp->Clear();
			commandList->ClearRenderTargetView(temp->currentFrameBuffer, game::Colors::DarkGray.rgba, 0, nullptr);

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
#if defined(GAME_DIRECTX12)
		if (geIsUsing(GAME_DIRECTX12))
		{
			game::RendererDX12* temp = geGetRenderer();
			temp->EndFrame();


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