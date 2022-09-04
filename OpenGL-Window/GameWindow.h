#pragma once
#include <Windows.h>
#include <string>

#include "GameHelpers.h"
#include "GameAttributes.h"

namespace game
{
	class Window
	{
	public:
		Window();
		bool CreateTheWindow();
		void SetAttributes(const Attributes attrib);
		void SetWindowTitle(const std::string title);
		void ToggleFullScreen();
		void DoMessagePump();
		HWND GetHandle();
	private:
		Attributes _attributes;

		// Windows only stuff
		RECT _windowPositionSave;
		HWND _windowHandle;
		MONITORINFO _monitorInfo = { 0 };
		static LRESULT CALLBACK _WindowEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}