#pragma once

#include "GameDefines.h"

#pragma region OpenGL
#if defined(GAME_OPENGL)
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")
#include "GameRendererGL.h"
#pragma endregion
#endif

#pragma region Vulkan
#if defined(GAME_VULKAN)
#include "vulkan/vulkan.hpp"
#pragma comment(lib, "vulkan-1.lib")
#include "GameRendererVK.h"
#endif
#pragma endregion


#pragma region DirectX9
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")
#include "GameRendererDX9.h"
#endif
#pragma endregion

#pragma region DirectX11
#if defined(GAME_DIRECTX11)
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include "GameRendererDX11.h"
#endif
#pragma endregion

#pragma region Engine
#include "GameAttributes.h"
#include "GameColor.h"
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
#include "GameShader.h"
#include "GameSpriteBatch.h"
#include "GameSpriteFont.h"
#include "GameSpriteSheet.h"
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