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
		bool SetWindowInfo(const std::string title, const uint32_t width, const uint32_t height, const bool fullScreen, const bool borderless);
		bool CreateTheWindow();
		void SetWindowTitle(const std::string title);
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
}