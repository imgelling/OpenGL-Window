#include <string>
#include "GameWindow.h"

#include "../../../Programming/GameLib2/GameErrors.h"
#include "GameEngine.h"


namespace game
{
	extern GameError lastError;

	Window::Window()
	{
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
		case WM_CLOSE:		if (enginePointer) enginePointer->isRunning = false; return 0;
		case WM_DESTROY:	PostQuitMessage(0); DestroyWindow(hWnd); return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool Window::CreateTheWindow()
	{
		WNDCLASS wc{};
		DWORD dwExStyle = 0;
		DWORD dwStyle = 0;
		int32_t windowWidth = 0;
		int32_t windowHeight = 0;
		HMONITOR monitorHandle = 0;
		MONITORINFO monitorInfo = { 0 };
		RECT rWndRect = { 0 };
		int32_t adjustedWidth = 0;
		int32_t adjustedHeight = 0;
		int32_t windowLeftPosition = 0;
		int32_t windowTopPosition = 0;


		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = WindowEventProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpszMenuName = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszClassName = Wide("GAME_ENGINE"); 
		RegisterClass(&wc);

		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		// Dimensions of the window
		windowWidth = _attributes.WindowWidth;
		windowHeight = _attributes.WindowHeight;

		// Get info about the monitor
		monitorHandle = MonitorFromWindow(_windowHandle, MONITOR_DEFAULTTONEAREST);
		monitorInfo = { sizeof(monitorInfo) };

		// If the window is fullscreen, change the style of the window
		if (_attributes.isWindowFullscreen)
		{
			dwExStyle = 0;
			dwStyle = WS_VISIBLE | WS_POPUP;

			// Get info about the monitor
			if (!GetMonitorInfo(monitorHandle, &monitorInfo))
			{
				lastError = { GameErrors::GameWindowsSpecific, "GetMonitorInfo failed." };
				return false;
			}

			// Save max size of monitor
			windowWidth = monitorInfo.rcMonitor.right;
			windowHeight = monitorInfo.rcMonitor.bottom;
		}

		// Adjust window size to account for title bar and edges
		rWndRect = { 0, 0, windowWidth, windowHeight };
		AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
		adjustedWidth = rWndRect.right - rWndRect.left;
		adjustedHeight = rWndRect.bottom - rWndRect.top;
		windowLeftPosition = 0;
		windowTopPosition = 0;

		// Center the window
		if (!_attributes.isWindowFullscreen)
		{
			windowLeftPosition = (monitorInfo.rcMonitor.right + (adjustedWidth >> 1)) >> 1;
			windowTopPosition = (monitorInfo.rcMonitor.bottom + (adjustedHeight >> 1)) >> 1;
		}

		// Create the actual window
		_windowHandle = CreateWindowEx(dwExStyle, Wide("GAME_ENGINE"), Wide(""), dwStyle,
			windowLeftPosition, windowTopPosition, adjustedWidth, adjustedHeight, NULL, NULL, GetModuleHandle(nullptr), this);
		if (!_windowHandle)
		{
			lastError = { GameErrors::GameWindowsSpecific, "Windows Error Number : " + std::to_string(GetLastError()) };
			return false;
		}

		// Set the window title
		SetWindowTitle(_attributes.WindowTitle);

		return true;
	}

	void Window::SetAttributes(const GameAttributes attrib)
	{
		_attributes = attrib;
	}

	void Window::SetWindowTitle(const std::string title)
	{
#ifdef UNICODE
		SetWindowText(_windowHandle, ConvertToWide(title).c_str());
#else
		SetWindowText(olc_hWnd, s.c_str());
#endif
	}

	void Window::DoMessagePump()
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
	
}