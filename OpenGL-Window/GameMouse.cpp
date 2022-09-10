#include "GameMouse.h"

namespace game
{
	Mouse::Mouse()
	{
		_wheelDelta = 0;
		_buttons = new bool[10];
		for (uint8_t button = 0; button < 10; button++)
		{
			_buttons[button] = false;
		}
	}

	Mouse::~Mouse()
	{
		delete[] _buttons;
	}

	bool Mouse::IsButtonPressed(const int32_t button)
	{
		if ((button < 0) || (button > 9)) return false;

		return _buttons[button];
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

	void Mouse::SetMouseState(const uint32_t button, const bool pressed)
	{
		_buttons[button] = pressed;
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