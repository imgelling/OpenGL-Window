#include <windows.h>
#include <iostream>

// Engine header
//#define GAME_USE_DEDICATED_GPU
#include "Game.h"


void game::Engine::Initialize()
{
	Attributes attrib;

	attrib.WindowTitle = "Spinning Triangle";
	attrib.GameVersion = "0.01";
	attrib.Framelock = 60;
	attrib.isVsyncOn = false;
	attrib.isDebugMode = false;
	attrib.isWindowFullscreen = true;
	//attrib.RenderingAPI = RenderAPI::Vulkan;
	SetAttributes(attrib);
}

void game::Engine::Shutdown()
{
}

void game::Engine::Update(const float msElapsed)
{
	static bool first = true;
	static double upsTime = 0.0f;
	static uint32_t updatesCounted = 0;

	upsTime += msElapsed;
	//std::cout << updatesCounted << "\n";
	updatesCounted++;
	if (upsTime >= 1000.0f)
	{
		//std::cout << "Updates per second :" << updatesCounted << "\n";
		SetWindowTitle("Spinning Triangle - " + std::to_string(updatesCounted) + " ups.");
		updatesCounted = 0;
		upsTime = upsTime - 1000.0f;
	}

	if (first)
	{
		// Testing swapping windowed to windowfullscreen borderless
		
		//// styles for windowed
		//DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		//DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		//// styles for fullscreen
		//if (!_attributes.isWindowFullscreen)
		//{
		//	dwExStyle = 0;
		//	dwStyle = WS_VISIBLE | WS_POPUP;
		//}
		//SetWindowLongPtr(enginePointer->_window.GetHandle(), GWL_STYLE, dwStyle);
		//SetWindowLongPtr(enginePointer->_window.GetHandle(), GWL_EXSTYLE, dwExStyle);
		//// Need to save state of window for swapping back and forth
		//// also probably need to re adjust for title bar and border (full to window)
		//SetWindowPos(enginePointer->_window.GetHandle(), 0, 0, 0, 1920, 1080, SWP_DRAWFRAME | SWP_FRAMECHANGED);
		//// seems to work
				
		//game::GL::wglSwapInterval(1);
		first = false;
	}
	if (keyboard.WasKeyReleased(VK_F11))
	{
		ToggleFullscreen();
	}
	if (keyboard.WasKeyReleased(VK_ESCAPE))
	{
		Stop();
	}
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
}

void game::Engine::Render(const float msElapsed)
{
	static double fpsTime = 0.0f;
	static uint32_t framesCounted = 0;

	fpsTime += msElapsed;
	framesCounted++;
	if (fpsTime >= 1000.0f)
	{
		SetWindowTitle("Spinning Triangle - " + std::to_string(framesCounted) + " fps.");
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

int main()
{
	game::GameLogger logger("Log.html");
	game::Engine engine(&logger);

	// Create the needed bits for the engine
	if (!engine.Create())
	{
		logger.Error(game::lastError);
		return EXIT_FAILURE;
	}
	
	// Just to see version number
	//std::cout << glGetString(GL_VERSION) << "\n";

	// Start the engine
	engine.Start();

	return EXIT_SUCCESS;
}