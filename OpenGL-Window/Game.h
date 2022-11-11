#pragma once


#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
#define GAME_OPENGL 1
#endif

#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL)
#define GAME_DIRECTX9 2
#endif

#if defined(GAME_SUPPORT_VULKAN) || defined(GAME_SUPPORT_ALL)
#define GAME_VULKAN 3
#endif


#pragma region Vulkan
#if defined(GAME_VULKAN)
#include "vulkan/vulkan.hpp"
#pragma comment(lib, "vulkan-1.lib")
#include "GameRendererVK.h"
#endif
#pragma endregion

#pragma region OpenGL
#if defined(GAME_OPENGL)
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")
#include "GameRendererGL.h"
#pragma endregion
#endif

#pragma region DirectX9
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")
#include "GameRendererDX9.h"
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
	// Pointer to the engine created
	Engine* enginePointer = nullptr;
	// GameEngine global error tracking
	GameError lastError;
	// Information about computer and graphics card
	SystemInfo systemInfo;
}