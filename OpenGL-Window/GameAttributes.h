#pragma once
#include "GameHelpers.h"

namespace game
{
	struct GameAttributes
	{
		std::string WindowTitle;// Title of the window created
		uint32_t WindowWidth;	// Width of the window created
		uint32_t WindowHeight;	// Height of the window created
		bool isWindowFullscreen;// Is the window created fullscreen
		bool isWindowBorderless;// Is the window created borderless
		uint8_t ContextMajor;	// Major version number (OpenGL only for now, may use for dx10,11,12)
		uint8_t ContextMinor;	// Minor version number (OpenGL)
		uint8_t RedSize;		// Size, in bits of red component of color depth
		uint8_t BlueSize;		// Size, in bits of blue component of color depth
		uint8_t GreenSize;		// Size, in bits of green component of color depth
		uint8_t AlphaSize;		// Size, in bits of alpha component of color depth
		uint8_t DoubleBuffer;	// Double buffer (Maybe triple... unsure), may need to be a bool
		uint8_t DepthSize;		// Size, in bits of depth buffer
		uint8_t MultiSamples;	// Multisampling, 0 is none, >0 enables multisampling
		bool isDebugMode;		// Enable debug mode with the renderer (may just be OpenGL only)
		uint8_t Framelock;		// A software lock on max frames per second
		RenderAPI RenderingAPI;	// Which rendering API are we using
		bool isVsync;			// Is vsync on
		GameAttributes();		// Loads some defaults intro structure
	};
}