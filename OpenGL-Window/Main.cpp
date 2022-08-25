#include <windows.h>
#include <iostream>

// Engine header
#include "Game.h"

// Vulkan 
#include <vulkan/vulkan.h>

void game::Engine::Initialize()
{
	GameAttributes attrib;
	attrib.WindowTitle = "Spinning Triangle";
	SetAttributes(attrib);
}

void game::Engine::Shutdown(void)
{
}

int main()
{
	game::Engine eng;

	eng.Initialize();

	if (!eng.Start())
	{
		std::cout << game::lastError;
		return -1;
	}
	
	// Just to see version number
	std::cout << glGetString(GL_VERSION) << "\n";

	// Get rid of boring black background color
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	
	// Game Loop
	do
	{
		// Catch messages from Windows
		eng.ProcessMessages();
		
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

		eng.Swap();
	} while (eng.isRunning);

	eng.Shutdown();

	return 0;
}