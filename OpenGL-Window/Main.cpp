// Engine header
#define GAME_USE_DEDICATED_GPU
//#define GAME_ENABLE_NETWORKING
//#define GAME_ENABLE_SHADERS
//#define GAME_SUPPORT_DIRECTX9
//#define GAME_SUPPORT_DIRECTX10
//#define GAME_SUPPORT_DIRECTX11
//#define GAME_SUPPORT_DIRECTX12
//#define GAME_SUPPORT_OPENGL
//#define GAME_SUPPORT_VULKAN 
#define GAME_SUPPORT_ALL
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

	//temp
	uint8_t* fontROM;

	Game(game::Logger& logger) : game::Engine(&logger)
	{
		fontROM = nullptr;
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
		//attributes.MultiSamples = 8; // Not implemented in most of DX, if any
		attributes.RenderingAPI = game::RenderAPI::DirectX9; 
		attributes.RenderingAPI = game::RenderAPI::DirectX10;
		attributes.RenderingAPI = game::RenderAPI::DirectX11;
		//attributes.RenderingAPI = game::RenderAPI::DirectX12;
		attributes.RenderingAPI = game::RenderAPI::OpenGL;
		//attributes.RenderingAPI = game::RenderAPI::Vulkan;
		
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
		if (!pixelMode.Initialize({ 320, 180 }))
		{
			geLogLastError();
		}

		// Setup sprite batch
		if (!spriteBatch.Initialize()) //2000000))
		{
			geLogLastError();
		}

		// Load font for output to screen
		if (!spriteFont.Load("Content/new.fnt", "Content/new.png"))
		{
			geLogLastError();
		}	

		fontROM = new uint8_t[128 * 48];
		FillMemory(fontROM,(uint8_t)0, (size_t)128 * (size_t)48);
		std::string data;
		data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
		data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
		data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
		data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
		data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
		data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
		data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
		data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
		data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
		data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
		data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
		data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
		data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
		data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
		data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
		data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

		//fontRenderable.Create(128, 48);

		int px = 0, py = 0;
		int32_t ox = ('a' - 32) % 16;
		int32_t oy = ('a' - 32) / 16;
		for (size_t b = 0; b < 1024; b += 4)
		{
			uint32_t sym1 = (uint32_t)data[b + 0] - 48;
			uint32_t sym2 = (uint32_t)data[b + 1] - 48;
			uint32_t sym3 = (uint32_t)data[b + 2] - 48;
			uint32_t sym4 = (uint32_t)data[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 24; i++)
			{
				uint32_t k = r & (1 << i) ? 255 : 0;
				//pixelMode.PixelClip(px, py, game::Color(k, k, k, k));
				fontROM[py * 128 + px] = k;
				if (++py == 48) { px++; py = 0; }
			}
			// render this into a quad (placing font into "ROM"
		}

		geMouse.UseMouseAcceleration(false);
	}

	void Shutdown()
	{
		geUnLoadTexture(spriteTexture);
		delete[] fontROM;
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
		geClear(GAME_FRAME_BUFFER_BIT | GAME_DEPTH_STENCIL_BUFFER_BIT, game::Colors::DarkGray);

		pixelMode.Clear(game::Colors::Blue);

		// Top and bottom
		pixelMode.HLineClip(340, -1, 0, game::Colors::Pink);
		pixelMode.HLineClip(-10, 409, 179, game::Colors::Pink);

		// Left and right
		pixelMode.VLineClip(0, -10, 300, game::Colors::Pink);
		pixelMode.VLineClip(319, 500, -10, game::Colors::Pink);

		//int32_t rx1 = random.RndRange(0, 340);
		//int32_t ry1 = random.RndRange(0, 340);

		//int32_t rx2 = random.RndRange(11, 329);
		//int32_t ry2 = random.RndRange(11, 329);
		//game::Color rndColor;
		//perftimer.Start("CircleClip");
		//for (uint32_t count = 0; count < 500000; count++)
		//{
		//	rx1 = random.RndRange(10, 310);;
		//	ry1 = random.RndRange(10, 180-11);;
		//	rndColor.Set(random.RndRange(0, 255), random.RndRange(0, 255), random.RndRange(0, 255), 255);
		//	pixelMode.CircleFilledClip(rx1, ry1, 10, rndColor);
		//	//rx2 = random.RndRange(11, 329);;
		//	//ry2 = random.RndRange(11, 329);;
		//}
		//perftimer.Stop("CircleClip");

		pixelMode.VPillClip(scaledMousePos.x, scaledMousePos.y, 40, 5, game::Colors::Green);
		pixelMode.VPillClip(scaledMousePos.x, scaledMousePos.y, 38, 4, game::Colors::White);
		
		// Weird diagonal
		pixelMode.LineClip(-20, -10, scaledMousePos.x, scaledMousePos.y, game::Colors::Pink);

		// Rectangle
		game::Recti rect;
		rect.left = 1;
		rect.top = 1;
		rect.right = 318;
		rect.bottom = 178;
		pixelMode.RectClip(rect, game::Colors::White);


		//int px = 1, py = 1;
		int32_t ox = ('A' - 32) % 16;
		int32_t oy = ('A' - 32) / 16;

		game::Recti rect2;
		rect2.Set(0, 0, 16*8+1, 6*8+1); // 128,48
		pixelMode.RectClip(rect2, game::Colors::Pink);

		int x = 20; int y = 20;
		for (uint32_t i = 0; i < 8; i++)
			for (uint32_t j = 0; j < 8; j++)
				if (fontROM[(j+oy*8)*128+(i+ox*8)] > 0)
					pixelMode.PixelClip(x+i, y+j, game::Colors::White);

		//// then draw
		//for (auto c : sText)
		//{
		//	if (c == '\n')
		//	{
		//		sx = 0; sy += 8 * scale;
		//	}
		//	else if (c == '\t')
		//	{
		//		sx += 8 * nTabSizeInSpaces * scale;
		//	}
		//	else
		//	{
		//		int32_t ox = (c - 32) % 16;
		//		int32_t oy = (c - 32) / 16;

		//		if (scale > 1)
		//		{
		//			for (uint32_t i = 0; i < 8; i++)
		//				for (uint32_t j = 0; j < 8; j++)
		//					if (fontRenderable.Sprite()->GetPixel(i + ox * 8, j + oy * 8).r > 0)
		//						for (uint32_t is = 0; is < scale; is++)
		//							for (uint32_t js = 0; js < scale; js++)
		//								Draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, col);
		//		}
		//		else
		//		{
		//			for (uint32_t i = 0; i < 8; i++)
		//				for (uint32_t j = 0; j < 8; j++)
		//					if (fontRenderable.Sprite()->GetPixel(i + ox * 8, j + oy * 8).r > 0)
		//						Draw(x + sx + i, y + sy + j, col);
		//		}
		//		sx += 8 * scale;
		//	}
		//}


		pixelMode.Render();

		spriteBatch.Begin();
		//double_t perSecond = 500000.0 / (perftimer.LastRun("CircleClip") / 1000000000.0);  // throws if not found
		//double_t millionPerSecond = perSecond / 1000.0 / 1000.0;
		for (int i = 0; i < 40; i++)
			spriteBatch.Draw(spriteTexture, {10 + (i * 100), 10}, game::Colors::White);
		spriteBatch.DrawString(spriteFont, "FPS : " + std::to_string(geGetFramesPerSecond()) + " UPS : " + std::to_string(geGetUpdatesPerSecond()) + " cpu : " + std::to_string(geGetCPUFrequency()) + "Mhz", 10, 200, game::Colors::White,2.0f);
		//spriteBatch.DrawString(spriteFont, "Random Circle(s) : " + std::to_string(millionPerSecond) + " million per second.", 10, 0, game::Colors::Red, 2.0f);
		spriteBatch.DrawString(spriteFont, "Window Pixel Size: " + std::to_string(geGetWindowSize().width) + "x" + std::to_string(geGetWindowSize().height), 10, 240, game::Colors::White, 2.0f);
		spriteBatch.DrawString(spriteFont, "PixelMode Pixel Size: " + std::to_string(pixelMode.GetPixelFrameBufferSize().width) + "x" + std::to_string(pixelMode.GetPixelFrameBufferSize().height), 10, 280, game::Colors::White, 2.0f);
		spriteBatch.DrawString(spriteFont, "Sprites Drawn Last Frame: " + std::to_string(spriteBatch.SpritesDrawnLastFrame()), 10, 320, game::Colors::White, 2.0f);
		spriteBatch.End();
	}

	// Sets up API stuff
	void Setup()
	{
#if defined (GAME_OPENGL)
		if (geIsUsing(GAME_OPENGL))
		{
			//glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
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