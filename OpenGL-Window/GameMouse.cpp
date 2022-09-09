#include "GameMouse.h"

namespace game
{
	Mouse::Mouse()
	{
		_wheelDelta = 0;
	}

	void Mouse::HandleMouseMove(const int32_t xPosition, const int32_t yPosition)
	{
		_positionOld = _position;

		_position = { xPosition, yPosition };

		_positionRelative = _position - _positionOld;
		
	}
	void Mouse::HandleMouseWheel(const int32_t delta)
	{
		_wheelDelta = delta;
	}

	void Mouse::ResetMouseValues()
	{
		_wheelDelta = 0;
		_positionRelative = { 0, 0 };
	}

	Pointi Mouse::GetPosition()
	{
		return _position;
	}
	
	Pointi Mouse::GetPositionRelative()
	{
		return _positionRelative;
	}
	
	int32_t Mouse::GetWheelDelta()
	{
		return _wheelDelta;
	}
	void Mouse::ShowMouse(const bool isShown)
	{
		ShowCursor(isShown);
	}
}