#pragma once
#include <stdint.h>
#include <Windows.h>
#include "GameMath.h"

namespace game
{
	class Mouse
	{
	public:
		Mouse();
		~Mouse();
		bool IsButtonPressed(const int32_t button);
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
		bool* _buttons;
	};
}
