#pragma once
#include <stdint.h>

namespace game
{
	class Mouse
	{
	public:
		Mouse();
		void HandleMouseMove(const int32_t xPosition, const int32_t yPosition);
		int32_t _wheel;
		int32_t _xPosition;
		int32_t _yPosition;
		int32_t _xPositionOld;
		int32_t _yPositionOld;
		int32_t _xPositionRelative;
		int32_t _yPositionRelative;
	private:

	};
}
