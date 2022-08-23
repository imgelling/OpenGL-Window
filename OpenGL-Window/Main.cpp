#include <windows.h>
#include <iostream>


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

	class Engine;

	static Engine* enginePointer;	// Do not use, maybe a struct or class that hides this


	class Window
	{
	public:
		Window();
		bool SetWindowInfo(const std::string title, const uint32_t width, const uint32_t height, const bool fullScreen, const bool borderless);
		bool CreateTheWindow();
		bool SetWindowTitle(const std::string title);
		void DoMessagePump();
		HWND GetHandle();
	private:
		std::string _windowTitle;
		uint32_t _windowWidth;
		uint32_t _windowHeight;
		bool _isFullScreen;
		bool _isBorderless;

		// Windows only stuff
		HWND _windowHandle;
		static LRESULT CALLBACK WindowEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
	// --- Window header Stop

		// --- Helpers Start
	enum class RenderAPI
	{
		OpenGL = 0,		// OpenGL, any context
		Vulkan,			// Vulkan, ??? about version
		DirectX			// DirectX 10, 11, or 12
	};

	std::wstring ConvertToWide(const std::string s)
	{
		uint32_t count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
		wchar_t* buffer = new wchar_t[count];
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
		std::wstring wideString(buffer);
		delete[] buffer;
		return wideString;
	}
	// --- Helpers Stop

	//--- Window Attrib header Start
	struct GameAttributes
	{
		uint8_t ContextMajor;	// Major version number (OpenGL only for now, may use for dx10,11,12)
		uint8_t ContextMinor;	// Minor version number (OpenGL)
		uint8_t RedSize;		// Size, in bits of red component of color depth
		uint8_t BlueSize;		// Size, in bits of blue component of color depth
		uint8_t GreenSize;		// Size, in bits of green component of color depth
		uint8_t AlphaSize;		// Size, in bits of alpha component of color depth
		uint8_t DoubleBuffer;	// Double buffer (Maybe triple... unsure), may need to be a bool
		uint8_t DepthSize;		// Size, in bits of depth buffer
		uint8_t MultiSamples;	// Multisampling, 0 is none, >0 enables multisampling
		bool DebugMode;			// Enable debug mode with the renderer (may just be OpenGL only)
		uint8_t Framelock;		// A software lock on max frames per second
		RenderAPI RenderingAPI;	// Which rendering API are we using
		GameAttributes();		// Loads some defaults intro structure
	};
	// --- GameAttribute header stop

	// --- Base renderer class Start
	class RendererBase
	{
	public:
		virtual bool CreateDevice(Window, bool vsync) { return false; };
		virtual void DestroyDevice(void) {};
		virtual void Swap(void) {};
	protected:
		bool _vSync = false;
		GameAttributes _attributes;
	};
	// --- Base renderer class Stop

	class GameAttribute;
	class Engine
	{
	public:
		bool isRunning = false;
		Engine() { enginePointer = this; }
		Window window;
		RendererBase r;
	private:
	};
	// --- Actual engine class Stop



	// --- GameAttrivute cpp start
	GameAttributes::GameAttributes()
	{
		ContextMajor = 0;
		ContextMinor = 0;
		RedSize = 8;
		BlueSize = 8;
		GreenSize = 8;
		AlphaSize = 8;
		DoubleBuffer = 0; // may need to be -1 as was set in GameLib1
		DepthSize = 0;
		MultiSamples = 0;
		DebugMode = false;
		Framelock = 0;
		RenderingAPI = RenderAPI::OpenGL; // Defaults to OpenGL
	}
	// --- GameAttribute cpp stop


// Macro to state a literal string is a wide string
#if defined(UNICODE) || defined(_UNICODE)
#define Wide(s) L##s
#else
#define Wide(s) s
#endif


	// GameEngine global error tracking
	GameError lastError;  

	// --- Window cpp Start
	Window::Window()
	{
		_windowWidth = 0;
		_windowHeight = 0;
		_isFullScreen = false;
		_isBorderless = false;
		_windowHandle = NULL;
	}
		
	
	LRESULT CALLBACK Window::WindowEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		case WM_CLOSE:		enginePointer->isRunning = false; return 0;
		case WM_DESTROY:	PostQuitMessage(0); DestroyWindow(hWnd); return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool Window::SetWindowInfo(const std::string title, const uint32_t width, const uint32_t height, const bool fullScreen, const bool borderless)
	{
		_windowTitle = title;
		_windowWidth = width;
		_windowHeight = height;
		_isFullScreen = fullScreen;
		_isBorderless = borderless;

		return true;
	}

	bool Window::CreateTheWindow()
	{
		WNDCLASS wc{};

		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = WindowEventProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpszMenuName = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszClassName = Wide("GAME_ENGINE"); // needs new macro
		RegisterClass(&wc);

		DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

		_windowHandle = CreateWindowEx(dwExStyle, Wide("GAME_ENGINE"), Wide(""), dwStyle,
			0, 0, _windowWidth, _windowHeight, NULL, NULL, GetModuleHandle(nullptr), this);
		if (!_windowHandle)
		{
			
			lastError = { GameErrors::GameWindowsSpecific, "Windows Error Number : "+std::to_string(GetLastError())};
			return false;
		}

		SetWindowTitle(_windowTitle);

		return true;
	}

	bool Window::SetWindowTitle(const std::string title)
	{
#ifdef UNICODE
		SetWindowText(_windowHandle, ConvertToWide(_windowTitle).c_str());
#else
		SetWindowText(olc_hWnd, s.c_str());
#endif
		return true;
	}

	void Window::DoMessagePump(void)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	HWND Window::GetHandle()
	{
		return _windowHandle;
	}
	// --- Window cpp Stop



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

	// Create rendering device
	if (!eng.r.CreateDevice(eng.window, true))
	{
		std::cout << game::lastError;
		eng.r.DestroyDevice();
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

		eng.r.Swap();
	} while (eng.isRunning);

	eng.r.DestroyDevice();
	return 0;
}