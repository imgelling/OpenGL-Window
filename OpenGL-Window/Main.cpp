#include <windows.h>
#include <iostream>

// Engine header
#include "Game.h"

// Vulkan 
#include <vulkan/vulkan.h>

// TODO ------
// add a cpp file for GamePerformanceTimer
// add a software framecap


void game::Engine::Initialize()
{
	GameAttributes attrib;
	attrib.WindowTitle = "Spinning Triangle";
	attrib.Framelock = 60;
	attrib.isVsync = false;
	SetAttributes(attrib);
}

void game::Engine::Shutdown()
{
}

void game::Engine::Update(const float msElapsed)
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
}

void game::Engine::Render(const float msElapsed)
{
	static float fpsTime = 0.0f;
	static float framesCounted = 0.0f;
	PerformanceTimer perf;

	perf.Start("Render");

	fpsTime += msElapsed;
	framesCounted++;
	if (fpsTime >= 1000.0f)
	{
		SetWindowTitle("Spinning Triangle - " + std::to_string(framesCounted) + " fps.");
		framesCounted = 0.0f;
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

	perf.Stop("Render");
}

int main()
{
	game::Engine eng;


	if (!eng.Create())
	{
		std::cout << game::lastError;
		return -1;
	}
	
	// Just to see version number
	std::cout << glGetString(GL_VERSION) << "\n";

	eng.Start();

	return 0;
}