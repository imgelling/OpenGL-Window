#include "GameKeyboard.h"

namespace game
{
	Keyboard::Keyboard()
	{
		
		_keyCurrentState = new bool[256];
		_keyOldState = new bool[256];
		for (uint16_t key = 0; key < 256; key++)
		{
			_keyCurrentState[key] = false;
			_keyOldState[key] = false;
		}
		
	}

	Keyboard::~Keyboard()
	{
		delete[] _keyCurrentState;
		delete[] _keyOldState;
	}

	void Keyboard::SetKeyState(const uint8_t key, const bool state)
	{
		if (_keyCurrentState[key] == state)
		{
			return;
		}

		_keyOldState[key] = _keyCurrentState[key];
		_keyCurrentState[key] = state;
	}

	bool Keyboard::WasKeyReleased(const uint8_t key)
	{
		bool currentState = _keyCurrentState[key];
		bool oldState = _keyOldState[key];

		_keyOldState[key] = _keyCurrentState[key];

		return (oldState && !currentState);
	}

	bool Keyboard::WasKeyPressed(const uint8_t key)
	{
		bool currentState = _keyCurrentState[key];
		bool oldState = _keyOldState[key];

		_keyOldState[key] = _keyCurrentState[key];

		return (!oldState && currentState);
	}

	bool Keyboard::IsKeyHeld(const uint8_t key)
	{
		static uint32_t temp = 0;

		bool currentState = _keyCurrentState[key];
		bool oldState = _keyOldState[key];

		_keyOldState[key] = _keyCurrentState[key];

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