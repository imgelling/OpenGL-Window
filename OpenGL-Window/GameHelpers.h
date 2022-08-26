#pragma once

#include <string>
#include <Windows.h>
#include "../../../Programming/GameLib2/GameErrors.h"

namespace game
{
	// --- Helpers Start
	enum class RenderAPI
	{
		OpenGL = 0,		// OpenGL, any context
		Vulkan,			// Vulkan, ??? about version
		DirectX			// DirectX 10, 11, or 12
	};

	inline std::wstring ConvertToWide(const std::string s)
	{
		uint32_t count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
		wchar_t* buffer = new wchar_t[count];
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
		std::wstring wideString(buffer);
		delete[] buffer;
		return wideString;
	}

	// Macro to state a literal string is a wide string
#if defined(UNICODE) || defined(_UNICODE)
#define Wide(s) L##s
#else
#define Wide(s) s
#endif

	// --- Helpers Stop
}