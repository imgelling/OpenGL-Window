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
		Vector2i GetPosition();
		Vector2i GetPositionRelative();
		int32_t GetWheelDelta();
	private:
		int32_t _wheelDelta;
		Vector2i _position;
		Vector2i _positionOld;
		Vector2i _positionRelative;
	};
}
