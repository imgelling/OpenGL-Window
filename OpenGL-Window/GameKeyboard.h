#if !defined(GAMEKEYBOARD_H)
#define GAMEKEYBOARD_H
#include <iostream>
#include <vector>

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


		void TextInputMode(const bool textInputMode);
		std::string GetTextInput();
		bool IsTextInput() const;
		uint32_t GetTabSize() const;
		void SetTabSize(const uint32_t tabSize);
		uint32_t GetCursorPosition() const;
	private:
		void _UpdateText(uint8_t key, uint8_t shiftedKey);
		bool* _keyCurrentState;
		bool* _keyOldState;
		bool _isTextInputMode;
		std::string _textInput;
		uint32_t _tabSize;
		uint32_t _cursorPosition;
		std::vector<std::string> _textBuffer;
		uint32_t _textBufferPosition;
		
	};

	inline Keyboard::Keyboard()
	{
		_keyCurrentState = new bool[256];
		_keyOldState = new bool[256];
		ZeroMemory(_keyCurrentState, 256);
		ZeroMemory(_keyOldState, 256);
		_isTextInputMode = false;
		_tabSize = 5;
		_cursorPosition = 0;
		_textBufferPosition = 0;
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
		return _textInput;
	}

	inline uint32_t Keyboard::GetCursorPosition() const
	{
		return _cursorPosition;
	}

	inline void Keyboard::TextInputMode(const bool textInputMode)
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

	inline void Keyboard::_UpdateText(uint8_t key, uint8_t shiftedKey)
	{
		if (_keyCurrentState[VK_SHIFT])
		{
			if (_cursorPosition < _textInput.length())
			{
				_textInput[_cursorPosition] = shiftedKey;
			}
			else
			{
				_textInput += shiftedKey;
			}
		}
		else
		{
			if (_cursorPosition < _textInput.length())
			{
				_textInput[_cursorPosition] = key;
			}
			else
			{
				_textInput += key;
			}
		}
		_cursorPosition++;
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

			// Cursor left
			if (key == geK_LEFT)
			{
				if (_cursorPosition)
				{
					_cursorPosition--;
				}
				return;
			}

			// Cursor right
			if (key == geK_RIGHT)
			{
				if (_cursorPosition < _textInput.length())
				{
					_cursorPosition++;
				}
				return;
			}

			// Move back in text entered history/buffer
			if (key == geK_UP)
			{
				if (_textBuffer.size() > 0)
				{
					_textBufferPosition--;
					_textInput = _textBuffer[_textBufferPosition];
					_cursorPosition = (uint32_t)_textInput.length();
				}
				return;
			}

			// Move forward in text entered history/buffer
			if (key == geK_DOWN)
			{
				if (_textBuffer.size() > 0)
				{
					// If we are not at the end, move forward in history/buffer
					if (_textBufferPosition < _textBuffer.size() - 1)
					{
						_textBufferPosition++;
						_textInput = _textBuffer[_textBufferPosition];
						_cursorPosition = (uint32_t)_textInput.length();
						return;
					}
					// If we are at the end, just give a blank line
					if (_textBufferPosition == _textBuffer.size() - 1)
					{
						_textInput = "";
						_textBufferPosition++;
						_cursorPosition = 0;
						return;
					}
				}
				return;
			}

			// If return is pressed, we need to store the current text
			// in the history/buffer and give a new line
			if (key == geK_RETURN)
			{
				_textBuffer.emplace_back(_textInput);
				_textInput = "";
				_cursorPosition = 0;
				_textBufferPosition = (uint32_t)_textBuffer.size();
				return;
			}


			// Is the key a letter?
			if (std::isalpha(key))
			{
				// These are scan codes and pass std::isalpha()
				// Ignore them.
				if ((key >= geK_F1) && (key <= geK_F12))
				{
					return;
				}
				_UpdateText(key + 32, key);
				return;
			}

			// Is the key a digit?
			if (std::isdigit(key))
			{
				uint8_t shiftedKey = 0;

				switch (key)
				{
				case geK_1: shiftedKey = '!'; break;
				case geK_2: shiftedKey = '@'; break;
				case geK_3: shiftedKey = '#'; break;
				case geK_4: shiftedKey = '$'; break;
				case geK_5: shiftedKey = '%'; break;
				case geK_6: shiftedKey = '^'; break;
				case geK_7: shiftedKey = '&'; break;
				case geK_8: shiftedKey = '*'; break;
				case geK_9: shiftedKey = '('; break;
				case geK_0: shiftedKey = ')'; break;
				default: break;
				}

				_UpdateText(key, shiftedKey);
				return;
			}

			// Space key
			if (key == geK_SPACE)
			{
				_UpdateText(' ', ' ');
				return;
			}

			// Minus and underscore key
			if (key == geK_MINUS)
			{
				_UpdateText('-', '_');
				return;
			}

			// Equal and plus key
			if (key == geK_PLUS)
			{
				_UpdateText('=', '+');
				return;
			}

			// Backspace key
			if (key == geK_BACK)
			{
				if (_textInput.length())
				{
					_textInput.erase((size_t)_cursorPosition-1, 1);
				}
				_cursorPosition--;
				return;
			}

			// Tab key
			if (key == VK_TAB)
			{
				for (uint32_t count = 0; count < _tabSize; count++)
				{
					_UpdateText(' ', ' ');
				}
				return;
			}

			// Comma and less than key
			if (key == geK_COMMA)
			{
				_UpdateText(',', '<');
				return;
			}

			// Period and greater than key
			if (key == geK_PERIOD)
			{
				_UpdateText('.', '>');
				return;
			}

			// Forward slash and question mark key
			if (key == geK_QUESTION)
			{
				_UpdateText('/', '?');
				return;
			}

			// Accent and tilde key
			if (key == geK_TILDE)
			{
				_UpdateText('`', '~');
				return;
			}

			// Left bracket and curley brace
			if (key == geK_LBRACKET)
			{
				_UpdateText('[', '{');
				return;
			}

			// Right bracket and curly brace
			if (key == geK_RBRACKET)
			{
				_UpdateText(']', '}');
				return;
			}

			// Back slash and pipe
			if (key == geK_BACKSLASH)
			{
				_UpdateText('\\', '|');
				return;
			}

			// Semi colon and colon
			if (key == geK_SEMICOLON)
			{
				_UpdateText(';', ':');
				return;
			}

			// Apostrophe and quote
			if (key == geK_APOSTROPHE)
			{
				_UpdateText('"', '\'');
				return;
			}


			// Delete
			if (key == geK_DELETE)
			{
				_textInput.erase(_cursorPosition, 1);
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

