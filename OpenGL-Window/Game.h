#pragma once



#pragma region Engine
#include "GameAttributes.h"
#include "GameEngine.h"
#include "../../../Programming/GameLib2/GameErrors.h"
#include "GameHelpers.h"
#include "GameRendererBase.h"
#include "GameRendererGL.h"
#include "GameWindow.h"
#pragma endregion

namespace game
{
	// Pointer to the engine created
	Engine* enginePointer = nullptr;
	// GameEngine global error tracking
	GameError lastError;
}