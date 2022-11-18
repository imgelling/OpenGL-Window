#pragma once

namespace game
{
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