#pragma once

#pragma region Opengl
#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma region WindowsOnly
#pragma comment(lib, "opengl32.lib")
#include "GameRendererGL.h"
#pragma endregion
#pragma endregion
#endif

#pragma region Vulkan
#if defined(GAME_SUPPORT_VULKAN) || defined(GAME_SUPPORT_ALL)
#include "vulkan/vulkan.hpp"
#pragma region WindowsOnly
#pragma comment(lib, "vulkan-1.lib")
#include "GameRendererVK.h"
#pragma endregion
#endif
#pragma endregion

#pragma region DirectX9
#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL)
#include <d3d9.h>
#pragma region WindowsOnly
#pragma comment (lib, "d3d9.lib")
#include "GameRendererDX9.h"
#pragma endregion
#endif
#pragma endregion

#pragma region Engine
#include "GameAttributes.h"
#include "GameColor.h"
#include "GameDefines.h"
#include "GameEngine.h"
#include "GameErrors.h"
#include "GameHelpers.h"
#include "GameImageLoader.h"
#include "GameKeyboard.h"
#include "GameLogger.h"
#include "GameMath.h"
#include "GameMouse.h"
#include "GamePerformanceTimer.h"
#include "GamePixelMode.h"
#include "GameRendererBase.h"
#include "GameShaderGL.h"
#include "GameSpriteBatch.h"
#include "GameSystemInfo.h"
#include "GameTerminal.h"
#include "GameTexture2D.h"
#include "GameTimer.h"
#include "GameWindow.h"
#pragma endregion

namespace game
{
	// Pointer to the engine created, only needed for windows message pump
	Engine* enginePointer = nullptr;
	// GameEngine global error tracking
	GameError lastError;

	SystemInfo systemInfo;
}