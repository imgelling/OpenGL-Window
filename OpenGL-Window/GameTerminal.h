#pragma once
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <iostream>
#include <string>


namespace game
{
	class Terminal
	{
	public:
		Terminal()
		{
			std::cout << resetAll;
#if _WIN32
			DWORD consoleMode = 0;
			DWORD err = 0;

			// Set output mode to handle virtual terminal sequences

			// Get the handle
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			err = GetLastError();
			if (err)
			{
				std::cout << "GetStdHandle() error " << err;
			}

			// Get the current console mode
			GetConsoleMode(consoleHandle, &initialConsoleMode);
			err = GetLastError();
			if (err)
			{
				std::cout << "GetConsoleMode() error " << err;
			}

			// Add the virtual terminal processing flag to the console mode
			// if it already isn't enabled.
			if (initialConsoleMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)
			{
				// It is already set, just copy it over
				consoleMode = initialConsoleMode;
			}
			else
			{
				// Not set, so we add the flag
				consoleMode = initialConsoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			}

			// Set the new console mode
			SetConsoleMode(consoleHandle, consoleMode);
			err = GetLastError();
			if (err)
			{
				std::cout << "SetConsoleMode() error " << err;
			}
#endif
		}
		~Terminal()
		{
#if _WIN32
			DWORD err = 0;

			// Reset the console back to initial state
			SetConsoleMode(consoleHandle, initialConsoleMode);
			err = GetLastError();
			if (err)
			{
				std::cout << "SetConsoleMode() error " << err;
			}
#endif
		}

		// Screen management codes
#pragma region Management
		const std::string resetAll = "\033[0m";
#pragma endregion

		// Cursor movement codes
#pragma region Cursor	
		const std::string MoveToHome = "\033[H";
		const std::string SetPosition(const uint16_t, const uint16_t);
		const void GetPosition(uint16_t&, uint16_t&);
		const std::string MoveUp(const uint16_t);
		const std::string MoveDown(const uint16_t);
		const std::string MoveRight(const uint16_t);
		const std::string MoveLeft(const uint16_t);
		const std::string MoveDownToBegining(const uint16_t); // needs better name
		const std::string MoveUpToBegining(const uint16_t);// needs better name
		const std::string MoveToColumn(const uint16_t);
		const std::string HideCursor = "\33[?25l";
		const std::string ShowCursor = "\33[?25h";
		const std::string SavePosition = "\0337";
		const std::string RestorePosition = "\0338";

#pragma endregion	

		// Erase codes
#pragma region Erase
		const std::string EraseLineToCursor = "\033[1K";
		const std::string EraseLineFromCursor = "\033[0K";
		const std::string EraseLine = "\033[2K";
		const std::string EraseScreenToCursor = "\033[1J";
		const std::string EraseScreenFromCursor = "\033[0J";
		const std::string EraseScreen = "\033[2J";
		const std::string EraseBackScroll = "\033[3J";
#pragma endregion

		// Text color changing codes
#pragma region Color
		const std::string DefaultColorFG = "\033[39m";
		const std::string DefaultColorBG = "\033[49m";
		const std::string ColorFG(const uint8_t);
		const std::string ColorBG(const uint8_t);
		const std::string Color256FG(const uint8_t);
		const std::string Color256BG(const uint8_t);
		const std::string Color24BitFG(const uint8_t, const uint8_t, const uint8_t);
		const std::string Color24BitBG(const uint8_t, const uint8_t, const uint8_t);
#pragma endregion	

		// Text style codes
#pragma region Style 
		// Check which ones work in windows 10 by default (font)
		const std::string Bold = "\033[1m";
		const std::string Dim = "\033[2m";
		const std::string Italic = "\033[3m";
		const std::string Underline = "\033[4m";
		const std::string Blink = "\033[5m";
		const std::string Invert = "\033[7m";
		const std::string Invisible = "\033[8m";
		const std::string Strikethrough = "\033[9m";
#pragma endregion

		// Text color codes
#pragma region ColorValues
		const uint8_t Black = 30;
		const uint8_t Red = 31;
		const uint8_t Green = 32;
		const uint8_t Yellow = 33;
		const uint8_t Blue = 34;
		const uint8_t Magenta = 35;
		const uint8_t Cyan = 36;
		const uint8_t White = 37;
#pragma endregion

	private:
#ifdef _WIN32
		// Holds the intial (at startup) mode of the console to reset with at shutdown
		DWORD initialConsoleMode = 0;
		// Holds the Windows handle of the console
		HANDLE consoleHandle = NULL;
#endif
	};
}
