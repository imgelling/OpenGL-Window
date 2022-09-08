#include "GameMouse.h"

namespace game
{
	Mouse::Mouse()
	{
		_wheel = 0;
		_xPosition = 0;
		_yPosition = 0;
		_xPositionOld = 0;
		_yPositionOld = 0;
		_xPositionRelative = 0;
		_yPositionRelative = 0;
	}

	void Mouse::HandleMouseMove(const int32_t xPosition, const int32_t yPosition)
	{
		_xPositionOld = _xPosition;
		_yPositionOld = _yPosition;
		_xPosition = xPosition;
		_yPosition = yPosition;

		_xPositionRelative = _xPosition - _xPositionOld;
		_yPositionRelative = _yPosition - _yPositionOld;
		
	}
}