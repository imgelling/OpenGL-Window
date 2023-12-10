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


		void TextInput(const bool textInputMode);
		std::string GetTextInput();
		bool IsTextInput() const;
		uint32_t GetTabSize() const;
		void SetTabSize(const uint32_t tabSize);
	private:
		bool* _keyCurrentState;
		bool* _keyOldState;
		bool _isTextInputMode;
		std::string _textInput;
		uint32_t _tabSize;
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
		_isTextInputMode = false;
		_tabSize = 5;
	}

	inline Keyboard::~Keyboard()
	{
		delete[] _keyCurrentState;
		delete[] _keyOldState;
	}

	inline bool Keyboard::IsTextInput() const
	{
		return _isTextInputMode;
	}

	inline std::string Keyboard::GetTextInput()
	{
		std::string temp = _textInput;
		//_textInput = "";
		return temp;
	}

	inline void Keyboard::TextInput(const bool textInputMode)
	{
		_isTextInputMode = textInputMode;
	}

	inline uint32_t Keyboard::GetTabSize() const
	{
		return _tabSize;
	}

	inline void Keyboard::SetTabSize(const uint32_t tabSize)
	{
		_tabSize = tabSize;
	}

	inline void Keyboard::SetKeyState(const uint8_t key, const bool state)
	{
		// Ignore repeats
		if (_keyCurrentState[key] == state)
		{
			return;
		}

		// Save the states
		_keyOldState[key] = _keyCurrentState[key];
		_keyCurrentState[key] = state;

		if (_isTextInputMode)
		{
			// Was a release captured? Ignore it
			if (!state)
			{
				return;
			}

			// Is the key a letter?
			if (std::isalpha(key))
			{
				if (_keyCurrentState[VK_SHIFT])
				{
					_textInput += key;
				}
				else
				{
					_textInput += key + 32;
				}
				return;
			}

			// Is the key a digit?
			if (std::isdigit(key))
			{
				if (_keyCurrentState[VK_SHIFT])
				{
					switch (key)
					{
					case geK_1: _textInput += '!'; break;
					case geK_2: _textInput += '@'; break;
					case geK_3: _textInput += '#'; break;
					case geK_4: _textInput += '$'; break;
					case geK_5: _textInput += '%'; break;
					case geK_6: _textInput += '^'; break;
					case geK_7: _textInput += '&'; break;
					case geK_8: _textInput += '*'; break;
					case geK_9: _textInput += '('; break;
					case geK_0: _textInput += ')'; break;
					default: break;
					}
					return;
				}
				_textInput += key;
				return;
			}

			// Space key
			if (key == geK_SPACE)
			{
				_textInput += " ";
				return;
			}

			// Minus and underscore key
			if (key == geK_MINUS)
			{
				if (_keyCurrentState[geK_SHIFT])
				{
					_textInput += '_';
				}
				else
				{
					_textInput += '-';
				}
				return;
			}

			// Equal and plus key
			if (key == geK_PLUS)
			{
				if (_keyCurrentState[geK_SHIFT])
				{
					_textInput += '+';
				}
				else
				{
					_textInput += '=';
				}
				return;
			}

			// Backspace key
			if (key == geK_BACK)
			{
				if (_textInput.length())
				{
					_textInput.erase(_textInput.length() - 1, 1);
				}
				return;
			}

			// Tab key
			if (key == VK_TAB)
			{
				for (uint32_t count = 0; count < _tabSize; count++)
				{
					_textInput+= " ";
				}
				return;
			}

			// Comma and less than key
			if (key == geK_COMMA)
			{
				if (_keyCurrentState[geK_SHIFT])
				{
					_textInput += '<';
				}
				else
				{
					_textInput += ',';
				}
				return;
			}

			// Period and greater than key
			if (key == geK_PERIOD)
			{
				if (_keyCurrentState[geK_SHIFT])
				{
					_textInput += '>';
				}
				else
				{
					_textInput += '.';
				}
				return;
			}
		}
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

