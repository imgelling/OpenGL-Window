#pragma once

#pragma region Opengl
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma region WindowsOnly
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")
// below is for flush (block till vsync), Windows only..maybe
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#pragma endregion
#pragma endregion

#include "GameRendererBase.h"

namespace game
{
	typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	wglSwapInterval_t* wglSwapInterval = nullptr;

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

		bool CreateDevice(Window window, const bool vsync)
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
}
