#include "GameTerminal.h"

namespace game
{

	//columnandrowstart a 1,1 being top left on linux, windows is 0,0--Note
	const std::string Terminal::SetPosition(const uint16_t column, const uint16_t row)
	{
#if _WIN32
		return "\033[" + std::to_string(row) + ";" + std::to_string(column) + "H";
#elif __linux
		return "\033[" + std::to_string(row + 1) + ";" + std::to_string(column + 1) + "H";
#endif
	}

	const void Terminal::GetPosition(uint16_t& column, uint16_t& row)
	{
#ifdef _WIN32
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
		GetConsoleScreenBufferInfo(h, &bufferInfo);
		column = bufferInfo.dwCursorPosition.X;
		row = bufferInfo.dwCursorPosition.Y;
#else
		column = 0;
		row = 0;
#endif
	}

	const std::string Terminal::MoveDownToBegining(const uint16_t row)
	{
		return "\033[" + std::to_string(row) + "E";
	}

	const std::string Terminal::MoveUpToBegining(const uint16_t row)
	{
		return "\033[" + std::to_string(row) + "F";
	}

	const std::string Terminal::MoveToColumn(const uint16_t col)
	{
		return "\033[" + std::to_string(col) + "G";
	}

	const std::string Terminal::MoveUp(const uint16_t number)
	{
		return "\033[" + std::to_string(number) + "A";
	}

	const std::string Terminal::MoveDown(const uint16_t number)
	{
		return "\033[" + std::to_string(number) + "B";
	}

	const std::string Terminal::MoveRight(const uint16_t number)
	{
		return "\033[" + std::to_string(number) + "C";
	}

	const std::string Terminal::MoveLeft(const uint16_t number)
	{
		return "\033[" + std::to_string(number) + "D";
	}

	const std::string Terminal::ColorFG(const uint8_t color)
	{
		return "\033[" + std::to_string(color) + "m";
	}

	const std::string Terminal::ColorBG(const uint8_t color)
	{
		return "\033[" + std::to_string(color + 10) + "m";
	}

	const std::string Terminal::Color256FG(const uint8_t color)
	{
		return "\033[38;5;" + std::to_string(color) + "m";
	}

	const std::string Terminal::Color256BG(const uint8_t color)
	{
		return "\033[48;5;" + std::to_string(color) + "m";
	}

	const std::string Terminal::Color24BitFG(const uint8_t r, const uint8_t g, const uint8_t b)
	{
		return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
	}

	const std::string Terminal::Color24BitBG(const uint8_t r, const uint8_t g, const uint8_t b)
	{
		return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
	}

}
