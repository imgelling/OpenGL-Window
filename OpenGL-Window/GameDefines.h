#pragma once

namespace game
{
#pragma region geEnable and geDisable defines
#define GAME_BLEND		(1 << 0)
#define GAME_CULL_FACE	(1 << 1)
#define GAME_TEXTURE_2D (1 << 2)
#pragma endregion

#pragma region blend sources
#define GAME_SRC_ALPHA (1 << 0)
#pragma endregion

#pragma region blend modes
#define GAME_ONE_MINUS_SRC_ALPHA (1 << 0)
#pragma endregion


}