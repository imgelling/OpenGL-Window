#pragma once

namespace game
{
#pragma region geEnable and geDisable defines
#define GAME_BLEND  (1 << 0)
#define GAME_CULL_FACE	(1 << 1)
#define GAME_TEXTURE_2D (1 << 2)
#pragma endregion

#pragma region blend sources
#define GAME_SRC_ALPHA (1 << 0)
#pragma endregion

#pragma region blend modes
#define GAME_ONE_MINUS_SRC_ALPHA (1 << 0)
#pragma endregion

#if defined(GAME_SUPPORT_OPENGL) || defined(GAME_SUPPORT_ALL)
#define GAME_OPENGL 1
#endif

#if defined(GAME_SUPPORT_DIRECTX9) || defined(GAME_SUPPORT_ALL)
#define GAME_DIRECTX9 2
#endif

#if defined(GAME_SUPPORT_DIRECTX11) || defined(GAME_SUPPORT_ALL)
#define GAME_DIRECTX11 4
#endif

#if defined(GAME_SUPPORT_VULKAN) || defined(GAME_SUPPORT_ALL)
#define GAME_VULKAN 3
#endif


}