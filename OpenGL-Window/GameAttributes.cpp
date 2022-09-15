#include "GameAttributes.h"

namespace game
{
	Attributes::Attributes()
	{
		WindowTitle = "";
		WindowWidth = 1280;
		WindowHeight = 720;
		WindowFullscreen = false;
		WindowBorderless = false;
		WindowVisible = true;
		ContextMajor = 3;
		ContextMinor = 2;
		RedSize = 8;
		BlueSize = 8;
		GreenSize = 8;
		AlphaSize = 8;
		StencilSize = 8;
		DoubleBuffer = 1; // may need to be -1 as was set in GameLib1
		GLBackwardsCompatible = true;
		DepthSize = 24; // 32 bit for amd only, 24 for nvidia
		MultiSamples = 1;
		DebugMode = false;
		Framelock = 0.0f;
		RenderingAPI = RenderAPI::OpenGL; // Defaults to OpenGL
		VsyncOn = true;
		GameVersion = "0";
	}
}