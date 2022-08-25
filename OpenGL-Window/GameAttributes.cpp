#include "GameAttributes.h"

namespace game
{
	// --- GameAttrivute cpp start
	GameAttributes::GameAttributes()
	{
		ContextMajor = 0;
		ContextMinor = 0;
		RedSize = 8;
		BlueSize = 8;
		GreenSize = 8;
		AlphaSize = 8;
		DoubleBuffer = 0; // may need to be -1 as was set in GameLib1
		DepthSize = 0;
		MultiSamples = 0;
		DebugMode = false;
		Framelock = 0;
		RenderingAPI = RenderAPI::OpenGL; // Defaults to OpenGL
	}
	// --- GameAttribute cpp stop
}