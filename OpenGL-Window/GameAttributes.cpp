#include "GameAttributes.h"

namespace game
{
	GameAttributes::GameAttributes()
	{
		WindowTitle = "";
		WindowWidth = 1280;
		WindowHeight = 720;
		isWindowFullscreen = false;
		isWindowBorderless = false;
		ContextMajor = 0;
		ContextMinor = 0;
		RedSize = 8;
		BlueSize = 8;
		GreenSize = 8;
		AlphaSize = 8;
		DoubleBuffer = 0; // may need to be -1 as was set in GameLib1
		DepthSize = 0;
		MultiSamples = 0;
		isDebugMode = false;
		Framelock = 0.0f;
		RenderingAPI = RenderAPI::OpenGL; // Defaults to OpenGL
		isVsync = true;
	}
}