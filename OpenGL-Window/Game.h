#pragma once

#pragma region Opengl
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma region WindowsOnly
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")
// below is for flush (block till vsync), Windows only..maybe
//#include <dwmapi.h>
//#pragma comment(lib, "Dwmapi.lib")
#pragma endregion
#pragma endregion

#pragma region Vulkan
#include "vulkan/vulkan.hpp"
#pragma region WindowsOnly
#pragma comment(lib, "vulkan-1.lib")
#pragma endregion
#pragma endregion

#pragma region Engine
#include "GameAttributes.h"
#include "GameEngine.h"
#include "../../../Programming/GameLib2/GameErrors.h"
#include "GameHelpers.h"
#include "GamePerformanceTimer.h"
#include "GameRendererBase.h"
#include "GameRendererGL.h"
#include "GameRendererVK.h"
#include "GameTimer.h"
#include "GameWindow.h"
#pragma endregion

namespace game
{
	// Pointer to the engine created
	Engine* enginePointer = nullptr;
	// GameEngine global error tracking
	GameError lastError;
}