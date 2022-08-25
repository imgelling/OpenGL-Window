#include <windows.h>
#include <iostream>

// Engine header
#include "Game.h"

// Vulkan 
#include <vulkan/vulkan.h>

void game::Engine::Initialize()
{
	// Set window info
	if (!window.SetWindowInfo("Spinning Triangle", 1280, 720, false, false))
	{
		std::cout << game::lastError;
	}

	// Create the window
	if (!window.CreateTheWindow())
	{
		std::cout << game::lastError;
	}

	// Set the renderer
	renderer = new game::RendererGL();

	// Create rendering device
	if (!renderer->CreateDevice(window, true))
	{
		std::cout << game::lastError;
		renderer->DestroyDevice();
	}
}

int main()
{
	game::Engine eng;

	eng.Initialize();
	
	// Just to see version number
	std::cout << glGetString(GL_VERSION) << "\n";

	// Get rid of boring black background color
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	
	// Game Loop
	do
	{
		// Catch messages from Windows
		eng.window.DoMessagePump();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glRotatef(1,1.0, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5, 0); // Pass first vertex
		
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.5, 0); // Pass second vertex
		
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0, 0.5); // Pass third vertex
		
		glEnd();

		eng.renderer->Swap();
	} while (eng.isRunning);

	eng.renderer->DestroyDevice();
	return 0;
}