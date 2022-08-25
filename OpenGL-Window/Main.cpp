#include <windows.h>
#include <iostream>

// Engine header
#include "Game.h"

// Vulkan 
#include <vulkan/vulkan.h>

int main()
{
	game::Engine eng;

	// Create the window
	if (!eng.window.SetWindowInfo("Spinning Triangle", 1280, 720, false, false))
	{
		std::cout << game::lastError;
	}

	if (!eng.window.CreateTheWindow())
	{
		std::cout << game::lastError;
	}

	eng.renderer = new game::RendererGL();

	// Create rendering device
	if (!eng.renderer->CreateDevice(eng.window, true))
	{
		std::cout << game::lastError;
		eng.renderer->DestroyDevice();
		return -1;
	}
	
	std::cout << glGetString(GL_VERSION) << "\n";


	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	// "Game Loop"
	do
	{
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
	delete eng.renderer;
	return 0;
}