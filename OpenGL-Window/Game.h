#pragma once

#include "GameAttributes.h"
#include "GameEngine.h"
#include "../../../Programming/GameLib2/GameErrors.h"
#include "GameHelpers.h"
#include "GameRendererBase.h"
#include "GameWindow.h"

namespace game
{
	Engine* enginePointer = nullptr;
	// GameEngine global error tracking
	GameError lastError;
}