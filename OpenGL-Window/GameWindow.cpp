#include "GameWindow.h"

#include "../../../Programming/GameLib2/GameErrors.h"
#include "GameEngine.h"


namespace game
{
	extern GameError lastError;

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
		case WM_CLOSE:		if (enginePointer) enginePointer->isRunning = false; return 0;
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

			lastError = { GameErrors::GameWindowsSpecific, "Windows Error Number : " + std::to_string(GetLastError()) };
			return false;
		}

		SetWindowTitle(_windowTitle);

		return true;
	}

	void Window::SetWindowTitle(const std::string title)
	{
#ifdef UNICODE
		SetWindowText(_windowHandle, ConvertToWide(title).c_str());
#else
		SetWindowText(olc_hWnd, s.c_str());
#endif
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
}