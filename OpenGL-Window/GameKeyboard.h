#pragma once
#include <stdint.h>
#include <iostream>

namespace game
{
	class Keyboard
	{
	public:
		Keyboard();
		~Keyboard();
		void SetKeyState(const uint8_t key, const  bool state);
		bool WasKeyReleased(const uint8_t key);
		bool WasKeyPressed(const uint8_t key);
		bool IsKeyHeld(const uint8_t key);
	private:
		bool* _keyState;
		bool* _keyOldState;
	};
}

