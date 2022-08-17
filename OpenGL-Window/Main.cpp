#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
// below needs shortened
#include "../../../Programming/GameLib/GameErrors.h"

#if defined(_WIN32) && !defined(__MINGW32__)
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")
#endif

// Added for opengl stuff
#include <vector>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

bool close = false;

namespace game
{
	class GameWindow
	{
	public:
		GameWindow();
		GameError LastError();
		bool SetWindowInfo(std::string title, const int width, const int height, const bool fullScreen, const bool borderless);
		bool CreateTheWindow();
		bool SetWindowTitle(std::string title);
		void DoMessagePump();
		HWND GetHandle();
	private:
		std::string _windowTitle;
		uint32_t _windowWidth;
		uint32_t _windowHeight;
		bool _isFullScreen;
		bool _isBorderless;
		GameError _lastError;

		std::wstring ConvertS2W(std::string s)
		{
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}

		// macro below needs renamed
#if defined(UNICODE) || defined(_UNICODE)
#define olcT(s) L##s
#else
#define olcT(s) s
#endif

		// Windows only stuff
		HWND _windowHandle;
		static LRESULT CALLBACK WindowEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	GameWindow::GameWindow()
	{
		_windowWidth = 0;
		_windowHeight = 0;
		_isFullScreen = false;
		_isBorderless = false;
		_windowHandle = NULL;
	}

	GameError GameWindow::LastError()
	{
		return _lastError;
	}
	
	LRESULT CALLBACK GameWindow::WindowEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			//case WM_MOUSEMOVE:
			//{
			//	// Thanks @ForAbby (Discord)
			//	uint16_t x = lParam & 0xFFFF; uint16_t y = (lParam >> 16) & 0xFFFF;
			//	int16_t ix = *(int16_t*)&x;   int16_t iy = *(int16_t*)&y;
			//	ptrPGE->olc_UpdateMouse(ix, iy);
			//	return 0;
			//}
			//case WM_SIZE:       ptrPGE->olc_UpdateWindowSize(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);	return 0;
			//case WM_MOUSEWHEEL:	ptrPGE->olc_UpdateMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));           return 0;
			//case WM_MOUSELEAVE: ptrPGE->olc_UpdateMouseFocus(false);                                    return 0;
			//case WM_SETFOCUS:	ptrPGE->olc_UpdateKeyFocus(true);                                       return 0;
			//case WM_KILLFOCUS:	ptrPGE->olc_UpdateKeyFocus(false);                                      return 0;
			//case WM_KEYDOWN:	ptrPGE->olc_UpdateKeyState(mapKeys[wParam], true);                      return 0;
			//case WM_KEYUP:		ptrPGE->olc_UpdateKeyState(mapKeys[wParam], false);                     return 0;
			//case WM_SYSKEYDOWN: ptrPGE->olc_UpdateKeyState(mapKeys[wParam], true);						return 0;
			//case WM_SYSKEYUP:	ptrPGE->olc_UpdateKeyState(mapKeys[wParam], false);						return 0;
			//case WM_LBUTTONDOWN:ptrPGE->olc_UpdateMouseState(0, true);                                  return 0;
			//case WM_LBUTTONUP:	ptrPGE->olc_UpdateMouseState(0, false);                                 return 0;
			//case WM_RBUTTONDOWN:ptrPGE->olc_UpdateMouseState(1, true);                                  return 0;
			//case WM_RBUTTONUP:	ptrPGE->olc_UpdateMouseState(1, false);                                 return 0;
			//case WM_MBUTTONDOWN:ptrPGE->olc_UpdateMouseState(2, true);                                  return 0;
			//case WM_MBUTTONUP:	ptrPGE->olc_UpdateMouseState(2, false);                                 return 0;
		case WM_CLOSE:		close = true;// ptrPGE->olc_Terminate();                                                return 0;
		case WM_DESTROY:	PostQuitMessage(0); DestroyWindow(hWnd);								return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool GameWindow::SetWindowInfo(std::string title, const int width, const int height, const bool fullScreen, const bool borderless)
	{
		_windowTitle = title;
		_windowWidth = width;
		_windowHeight = height;
		_isFullScreen = fullScreen;
		_isBorderless = borderless;

		return true;
	}

	bool GameWindow::CreateTheWindow()
	{
		WNDCLASS wc{};

		_lastError.Clear();

		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = WindowEventProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpszMenuName = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszClassName = olcT("GAME_ENGINE"); // needs new macro
		RegisterClass(&wc);

		DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

		_windowHandle = CreateWindowEx(dwExStyle, olcT("GAME_ENGINE"), olcT(""), dwStyle,
			0, 0, _windowWidth, _windowHeight, NULL, NULL, GetModuleHandle(nullptr), this);
		if (!_windowHandle)
		{
			_lastError = { GameErrors::GameWindowsSpecific, "Windows Error Number : "+std::to_string(GetLastError())};
			return false;
		}

		SetWindowTitle(_windowTitle);

		return true;
	}

	bool GameWindow::SetWindowTitle(std::string title)
	{
#ifdef UNICODE
		SetWindowText(_windowHandle, ConvertS2W(_windowTitle).c_str());
#else
		SetWindowText(olc_hWnd, s.c_str());
#endif
		return true;
	}

	void GameWindow::DoMessagePump(void)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	HWND GameWindow::GetHandle()
	{
		return _windowHandle;
	}


	// OpenGL stuff
	typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	static wglSwapInterval_t* wglSwapInterval = nullptr;
	class Renderer
	{
//#define CALLSTYLE __stdcall
//#define OGL_LOAD(t, n) (t*)wglGetProcAddress(#n)
	public:
		typedef HDC glDeviceContext_t;
		typedef HGLRC glRenderContext_t;
		glDeviceContext_t glDeviceContext = NULL;
		glRenderContext_t glRenderContext = NULL;

		GameError LastError()
		{
			return _lastError;
		}

		// need attribute struct for window
		// make window shared pointer
		bool CreateDevice(GameWindow window, bool bVSYNC) 
		{
			_lastError.Clear();
			// Create Device Context
			glDeviceContext = GetDC(window.GetHandle());// (HWND)(params[0]));
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
				_lastError = { GameErrors::GameWindowsSpecific, "ChoosePixelFormat Failed." };
				return false;
			}
			SetPixelFormat(glDeviceContext, pf, &pfd);

			glRenderContext = wglCreateContext(glDeviceContext);
			if (!glRenderContext)
			{
				_lastError = { GameErrors::GameOpenGLSpecific, "wglCreateContext Failed." };
				return false;
			}
			wglMakeCurrent(glDeviceContext, glRenderContext);

			// Set Vertical Sync
			wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT"); //OGL_LOAD(wglSwapInterval_t, "wglSwapIntervalEXT");
			if (wglSwapInterval == NULL)
			{
				_lastError = { GameErrors::GameOpenGLSpecific, "Loading SwapInterval Failed" };
				return false;
			}
			if (wglSwapInterval && !bVSYNC) wglSwapInterval(0);
			else wglSwapInterval(1); // need to make sure swapinteral worked
			_vSync = bVSYNC;
			return true;
		}
		void DestroyDevice()
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(glRenderContext);

		}
		void Swap()
		{
			SwapBuffers(glDeviceContext);
			if (_vSync) DwmFlush(); // blocks till next present
		}
	private:
		GameError _lastError;
		bool _vSync = false;
	};

}

int main()
{
	game::GameWindow window;
	game::GameError error;
	game::Renderer renderer;

	// Create the window
	if (!window.SetWindowInfo("Test Name", 1280, 720, false, false))
	{
		std::cout << window.LastError();
	}
	if (!window.CreateTheWindow())
	{
		std::cout << window.LastError();
	}

	// Create rendering device
	if (!renderer.CreateDevice(window, true))
	{
		std::cout << renderer.LastError();
		renderer.DestroyDevice();
		return -1;
	}

	// "Game Loop"
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	do
	{
		window.DoMessagePump();

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

		renderer.Swap();
	} while (!close);

	renderer.DestroyDevice();
	return 0;
}