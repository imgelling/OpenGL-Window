#include <string>
#include <dwmapi.h>
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

	LRESULT CALLBACK Window::_WindowEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			//case WM_MOUSEWHEEL:	ptrPGE->olc_UpdateMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));           return 0;
			//case WM_MOUSELEAVE: ptrPGE->olc_UpdateMouseFocus(false);                                    return 0;
			//case WM_SETFOCUS:	ptrPGE->olc_UpdateKeyFocus(true);                                       return 0;
			//case WM_KILLFOCUS:	ptrPGE->olc_UpdateKeyFocus(false);                                      return 0;
		case WM_SIZE: enginePointer->HandleWindowResize(lParam & 0xFFF, (lParam >> 16) & 0xFFFF); return 0;
		case WM_KEYDOWN: enginePointer->keyboard.SetKeyState((uint8_t)wParam, true); return 0;
		case WM_KEYUP: enginePointer->keyboard.SetKeyState((uint8_t)wParam, false); return 0;
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
		/*MONITORINFO monitorInfo = { 0 };*/
		RECT rWndRect = { 0 };
		int32_t adjustedWidth = 0;
		int32_t adjustedHeight = 0;
		int32_t windowLeftPosition = 0;
		int32_t windowTopPosition = 0;


		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = _WindowEventProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpszMenuName = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszClassName = Wide("GAME_ENGINE"); 
		RegisterClass(&wc);

		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		if (!_attributes.isWindowVisible)
		{
			dwExStyle = 0;
			dwStyle = 0;
		}
		// Dimensions of the window
		windowWidth = _attributes.WindowWidth;
		windowHeight = _attributes.WindowHeight;

		// Get info about the monitor
		monitorHandle = MonitorFromWindow(_windowHandle, MONITOR_DEFAULTTONEAREST);
		_monitorInfo = { sizeof(_monitorInfo) };

		// Get info about the monitor
		if (!GetMonitorInfo(monitorHandle, &_monitorInfo))
		{
			lastError = { GameErrors::GameWindowsSpecific, "GetMonitorInfo failed." };
			return false;
		}
		
		// pretend we are making a window -- added for toggle fullscreen
		if (_attributes.isWindowVisible)
		{
			rWndRect = { 0, 0, windowWidth, windowHeight };
			AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
			adjustedWidth = rWndRect.right - rWndRect.left;
			adjustedHeight = rWndRect.bottom - rWndRect.top;
			windowLeftPosition = 0;
			windowTopPosition = 0;
			_windowPositionSave.left = ((_monitorInfo.rcMonitor.right >> 1) - (adjustedWidth >> 1));
			_windowPositionSave.top = ((_monitorInfo.rcMonitor.bottom >> 1) - (adjustedHeight >> 1));
			_windowPositionSave.right = _windowPositionSave.left + adjustedWidth;
			_windowPositionSave.bottom = _windowPositionSave.top + adjustedHeight;
			// to save the information
		}

		// If the window is fullscreen, change the style of the window
		if (_attributes.isWindowFullscreen)
		{
			dwExStyle = 0;
			dwStyle = WS_VISIBLE | WS_POPUP;

			// Save max size of monitor
			windowWidth = _monitorInfo.rcMonitor.right;
			windowHeight = _monitorInfo.rcMonitor.bottom;
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
			windowLeftPosition = ((_monitorInfo.rcMonitor.right >> 1) - (adjustedWidth >> 1));
			windowTopPosition = ((_monitorInfo.rcMonitor.bottom >> 1) - (adjustedHeight >> 1));
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

	void Window::SetAttributes(const Attributes attrib)
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

	void Window::ToggleFullScreen()
	{
		DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		_attributes.isWindowFullscreen = !_attributes.isWindowFullscreen;


		// swapping to fullscreen
		if (_attributes.isWindowFullscreen)
		{
			dwExStyle = 0;
			dwStyle = WS_VISIBLE | WS_POPUP;
			GetWindowRect(_windowHandle, &_windowPositionSave);

		}

		// Set window styles
		SetWindowLongPtr(_windowHandle, GWL_EXSTYLE, dwExStyle);
		SetWindowLongPtr(_windowHandle, GWL_STYLE, dwStyle);

		// Swapping to window
		if (!_attributes.isWindowFullscreen)
		{
			
			int32_t width = _windowPositionSave.right -_windowPositionSave.left;
			int32_t height = _windowPositionSave.bottom -_windowPositionSave.top;
			// Move window to old position for window
			SetWindowPos(_windowHandle, 0, _windowPositionSave.left, _windowPositionSave.top, width, height, SWP_DRAWFRAME | SWP_FRAMECHANGED);
		}
		else // swapping to fullscreen
		{
			// Move window to corners for fullscreen
			SetWindowPos(_windowHandle, 0, 0, 0, _monitorInfo.rcMonitor.right, _monitorInfo.rcMonitor.bottom, SWP_DRAWFRAME | SWP_FRAMECHANGED);
		}


	}

	HWND Window::GetHandle()
	{
		return _windowHandle;
	}
	
}