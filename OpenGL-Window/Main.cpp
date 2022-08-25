#include <windows.h>
#include <iostream>


// Temp header
#include "Game.h"


// OpenGL
#include <gl/GL.h>
#include <gl/GLU.h>
// Windows
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")


// Vulkan 
#include <vulkan/vulkan.h>

// below needs shortened in final
#include "../../../Programming/GameLib2/GameErrors.h"



// below is for flush (block till vsync), Windows only..maybe
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")



namespace game
{
	// --- Tests for window msg calls Start

	//class Engine;

	//static Engine* enginePointer;	// Do not use, maybe a struct or class that hides this

















	



	// --- OpenGL Start
	
	typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	static wglSwapInterval_t* wglSwapInterval = nullptr;

	class RendererGL : public RendererBase
	{
	public:
		typedef HDC glDeviceContext_t;
		typedef HGLRC glRenderContext_t;
		glDeviceContext_t glDeviceContext = NULL;
		glRenderContext_t glRenderContext = NULL;

		// need attribute struct for window
		// make window shared pointer, maybe could just be in game namespace
		// NEW - thinking this will be in engine class

		bool CreateDevice(Window window, const bool vsync) override 
		{
			// Create Device Context
			glDeviceContext = GetDC(window.GetHandle());
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};

			int pf = 0;
			pf = ChoosePixelFormat(glDeviceContext, &pfd);
			if (!pf)
			{
				lastError = { GameErrors::GameWindowsSpecific, "ChoosePixelFormat Failed." };
				return false;
			}
			SetPixelFormat(glDeviceContext, pf, &pfd);

			// Create the OpenGL rendering context
			glRenderContext = wglCreateContext(glDeviceContext);
			if (!glRenderContext)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "wglCreateContext Failed." };
				return false;
			}
			wglMakeCurrent(glDeviceContext, glRenderContext);

			// Set Vertical Sync
			wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");
			if (wglSwapInterval == NULL)
			{
				lastError = { GameErrors::GameOpenGLSpecific, "Loading SwapInterval Failed" };
				return false;
			}
			if (vsync)
				wglSwapInterval(0);
			else 
				wglSwapInterval(1); 
			_vSync = vsync;

			// Engine is now running
			enginePointer->isRunning = true;

			return true;
		}
		void DestroyDevice() override
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(glRenderContext);

		}
		void Swap() override
		{
			SwapBuffers(glDeviceContext);
			if (_vSync) DwmFlush(); // blocks till next present
		};
	private:
		};
	// --- OpenGL Stop

	// --- Vulkan Start
	// --- Vulkan End
}

int main()
{
	game::Engine eng;
	//game::Window window;
	//game::RendererGL renderer;

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