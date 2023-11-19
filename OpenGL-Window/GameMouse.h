#pragma once
#include "GameMath.h"

namespace game
{
	class Mouse
	{
	public:
		Mouse();
		~Mouse();
		bool IsButtonHeld(const int32_t button);
		bool WasButtonPressend(const int32_t button);
		bool WasButtonReleased(const int32_t button);
		void HandleMouseMove(const int32_t xPosition, const int32_t yPosition);
		void HandleMouseWheel(const int32_t delta);
		void SetMouseState(const uint32_t button, const bool pressed);
		void ResetMouseValues();
		Pointi GetPosition();
		Pointi GetPositionRelative();
		int32_t GetWheelDelta();
		void ShowMouse(const bool isShown);
	private:
		int32_t _wheelDelta;
		Pointi _position;
		Pointi _positionOld;
		Pointi _positionRelative;
		bool* _currentButtonState;
		bool* _oldButtonState;
	};

	inline Mouse::Mouse()
	{
		_wheelDelta = 0;
		_currentButtonState = new bool[10];
		_oldButtonState = new bool[10];
		for (uint8_t button = 0; button < 10; button++)
		{
			_currentButtonState[button] = false;
			_oldButtonState[button] = false;
		}
	}

	inline Mouse::~Mouse()
	{
		delete[] _currentButtonState;
		delete[] _oldButtonState;
	}

	inline bool Mouse::IsButtonHeld(const int32_t button)
	{
		if ((button < 0) || (button > 9)) return false;

		return _currentButtonState[button];
	}

	inline bool Mouse::WasButtonPressend(const int32_t button)
	{
		return false;
	}

	inline bool Mouse::WasButtonReleased(const int32_t button)
	{
		return false;
	}

	inline void Mouse::HandleMouseMove(const int32_t xPosition, const int32_t yPosition)
	{
		_positionOld = _position;

		_position = { xPosition, yPosition };

		_positionRelative = _position - _positionOld;

	}
	
	inline void Mouse::HandleMouseWheel(const int32_t delta)
	{
		_wheelDelta = delta;
	}

	inline void Mouse::SetMouseState(const uint32_t button, const bool pressed)
	{
		_oldButtonState[button] = _currentButtonState[button];
		_currentButtonState[button] = pressed;
	}

	inline void Mouse::ResetMouseValues()
	{
		_wheelDelta = 0;
		_positionRelative = { 0, 0 };
	}

	inline Pointi Mouse::GetPosition()
	{
		return _position;
	}

	inline Pointi Mouse::GetPositionRelative()
	{
		return _positionRelative;
	}

	inline int32_t Mouse::GetWheelDelta()
	{
		return _wheelDelta;
	}

	inline void Mouse::ShowMouse(const bool isShown)
	{
#if defined(_WIN32)
		ShowCursor(isShown);
#elif defined(__linux__)
		// show mouse
#endif
	}
}
