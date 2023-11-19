#if !defined(GAMEKEYBOARD_H)
#define GAMEKEYBOARD_H
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
		bool* _keyCurrentState;
		bool* _keyOldState;
	};

	inline Keyboard::Keyboard()
	{

		_keyCurrentState = new bool[256];
		_keyOldState = new bool[256];
		for (uint16_t key = 0; key < 256; key++)
		{
			_keyCurrentState[key] = false;
			_keyOldState[key] = false;
		}

	}

	inline Keyboard::~Keyboard()
	{
		delete[] _keyCurrentState;
		delete[] _keyOldState;
	}

	inline void Keyboard::SetKeyState(const uint8_t key, const bool state)
	{
		if (_keyCurrentState[key] == state)
		{
			return;
		}

		_keyOldState[key] = _keyCurrentState[key];
		_keyCurrentState[key] = state;
	}

	inline bool Keyboard::WasKeyReleased(const uint8_t key)
	{
		bool currentState = _keyCurrentState[key];
		bool oldState = _keyOldState[key];

		_keyOldState[key] = _keyCurrentState[key];

		return (oldState && !currentState);
	}

	inline bool Keyboard::WasKeyPressed(const uint8_t key)
	{
		bool currentState = _keyCurrentState[key];
		bool oldState = _keyOldState[key];

		_keyOldState[key] = _keyCurrentState[key];

		return (!oldState && currentState);
	}

	inline bool Keyboard::IsKeyHeld(const uint8_t key)
	{
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

#endif

