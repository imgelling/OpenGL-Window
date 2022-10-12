#pragma once

#include <string>
#include <Windows.h>
#include "GameErrors.h"

namespace game
{
	// --- Helpers Start
	enum class RenderAPI
	{
		OpenGL = 0,		// OpenGL
		Vulkan,			// Vulkan
		DirectX9,		// DirectX
		DirectX10,
		DirectX11,
		DirectX12
	};

	inline std::wstring ConvertToWide(const std::string s)
	{
		uint32_t count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
		wchar_t* _buffer = new wchar_t[count];
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, _buffer, count);
		std::wstring wideString(_buffer);
		delete[] _buffer;
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