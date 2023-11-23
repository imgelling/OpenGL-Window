#if !defined(GAME_H)
#define GAME_H

#define GAME_FRAME_BUFFER_BIT (1)
#define GAME_DEPTH_STENCIL_BUFFER_BIT (1 << 1)

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// If user has not chosen a renderer, default to OpenGL
#if !defined(GAME_SUPPORT_DIRECTX12)
#if !defined(GAME_SUPPORT_DIRECTX11)
#if !defined(GAME_SUPPORT_DIRECTX10)
#if !defined(GAME_SUPPORT_DIRECTX9)
#if !defined(GAME_SUPPORT_OPENGL)
#if !defined(GAME_SUPPORT_VULKAN)
#if !defined(GAME_SUPPORT_ALL)
#define GAME_SUPPORT_OPENGL
#endif
#endif
#endif
#endif
#endif
#endif
#endif


#if defined(GAME_SUPPORT_DIRECTX12) || defined(GAME_SUPPORT_ALL)
#define GAME_DIRECTX12 5
#endif

#if defined(GAME_SUPPORT_DIRECTX11) || defined(GAME_SUPPORT_ALL)
#define GAME_DIRECTX11 4
#endif

#if defined(GAME_SUPPORT_DIRECTX10) || defined(GAME_SUPPORT_ALL)
#define GAME_DIRECTX10 6
#endif

#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL)
#define GAME_DIRECTX9 2
#endif

#include "GameWindowsKeys.h"

#endif // _WIN32

#if defined(GAME_SUPPORT_ALL)

#endif

#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
#define GAME_OPENGL 1
#endif

#if defined(GAME_SUPPORT_VULKAN) || defined(GAME_SUPPORT_ALL)
#define GAME_VULKAN 3
#endif

#if defined(_WIN32)

#pragma region DirectX12
#if defined(GAME_DIRECTX12)
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <initguid.h>
#include <wrl.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#include "GameRendererDX12.h"
#endif
#pragma endregion

#pragma region DirectX11
#if defined(GAME_DIRECTX11)
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "GameRendererDX11.h"
#endif
#pragma endregion

#pragma region DirectX10
#if defined(GAME_DIRECTX10)
#include <d3d10.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "GameRendererDX10.h"
#endif
#pragma endregion

#pragma region DirectX9
#if defined(GAME_DIRECTX9)
#include <d3d9.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "GameRendererDX9.h"
#endif
#pragma endregion

#endif // _WIN32

#pragma region Vulkan
#if defined(GAME_VULKAN)
#if defined (_WIN32)
#include "vulkan/vulkan.hpp"
#pragma comment(lib, "vulkan-1.lib")
#elif define(__linux__)

#endif
#include "GameRendererVK.h"
#endif
#pragma endregion

#pragma region OpenGL
#if defined(GAME_OPENGL)
#include <GL/gl.h>
#if defined(_WIN32)
#pragma comment(lib, "opengl32.lib")
#elif defined(__linux__)

#endif
#include "GameRendererGL.h"
#pragma endregion
#endif

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
#if defined(GAME_ENABLE_NETWORKING)
#include "GameNetwork.h"
#endif
#include "GamePerformanceTimer.h"
#include "GamePixelMode.h"
#include "GameRandom.h"
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

#endif