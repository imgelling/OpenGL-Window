#pragma once
#include <stdint.h>
#include "GameMath.h"

namespace game
{
	class Mouse
	{
	public:
		Mouse();
		void HandleMouseMove(const int32_t xPosition, const int32_t yPosition);
		void HandleMouseWheel(const int32_t delta);
		void ResetMouseValues();
		Pointi GetPosition();
		Pointi GetPositionRelative();
		int32_t GetWheelDelta();
	private:
		int32_t _wheelDelta;
		Pointi _position;
		Pointi _positionOld;
		Pointi _positionRelative;
	};
}
