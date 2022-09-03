#include "GameKeyboard.h"

namespace game
{
	Keyboard::Keyboard()
	{
		
		_keyState = new bool[256];
		_keyOldState = new bool[256];
		for (uint16_t key = 0; key < 256; key++)
		{
			_keyState[key] = false;
			_keyOldState[key] = false;
		}
		
	}

	Keyboard::~Keyboard()
	{
		delete[] _keyState;
		delete[] _keyOldState;
	}

	void Keyboard::SetKeyState(const uint8_t key, const bool state)
	{
		if (_keyState[key] == state)
		{
			return;
		}

		_keyOldState[key] = _keyState[key];
		_keyState[key] = state;
	}

	bool Keyboard::WasKeyReleased(const uint8_t key)
	{
		bool currentState = _keyState[key];
		bool oldState = _keyOldState[key];

		_keyOldState[key] = _keyState[key];

		return (oldState && !currentState);
	}

	bool Keyboard::WasKeyPressed(const uint8_t key)
	{
		bool currentState = _keyState[key];
		bool oldState = _keyOldState[key];

		_keyOldState[key] = _keyState[key];

		return (!oldState && currentState);
	}

	bool Keyboard::IsKeyHeld(const uint8_t key)
	{
		static uint32_t temp = 0;

		bool currentState = _keyState[key];
		bool oldState = _keyOldState[key];

		_keyOldState[key] = _keyState[key];

		if (currentState && oldState)
		{
			// current state and old state are true, so the key is held
			return true;
		}
		else if (!oldState && currentState)
		{
			// Still consider key held if this is the first time pressed
			return true;
		}
		return false;

	}
}