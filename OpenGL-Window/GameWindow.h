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
		void SetAttributes(const GameAttributes attrib);
		void SetWindowTitle(const std::string title);
		void DoMessagePump();
		HWND GetHandle();
	private:
		GameAttributes _attributes;

		// Windows only stuff
		HWND _windowHandle;
		static LRESULT CALLBACK WindowEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}