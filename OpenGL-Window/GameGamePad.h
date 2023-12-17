#if !defined(GAMECONTROLLER_H)
#define GAMECONTROLLER_H

#include <stdint.h>
#include <Xinput.h>
#include <iostream>

#include "GameMath.h"

constexpr auto GAME_GAME_PAD_1 = 0;
constexpr auto GAME_GAME_PAD_2 = 1;
constexpr auto GAME_GAME_PAD_3 = 2;
constexpr auto GAME_GAME_PAD_4 = 3;


namespace game
{
	struct Controller
	{

	};


	typedef struct _XINPUT_GAMEPAD {
		WORD  wButtons;
		BYTE  bLeftTrigger;
		BYTE  bRightTrigger;
		SHORT sThumbLX;
		SHORT sThumbLY;
		SHORT sThumbRX;
		SHORT sThumbRY;
	} XINPUT_GAMEPAD, * PXINPUT_GAMEPAD;

#define geG_A XINPUT_GAMEPAD_A
#define geG_B XINPUT_GAMEPAD_B
#define geG_X XINPUT_GAMEPAD_X
#define geG_Y XINPUT_GAMEPAD_Y
#define geG_L_BUMPER XINPUT_GAMEPAD_LEFT_SHOULDER
#define geG_R_BUMBER XINPUT_GAMEPAD_RIGHT_SHOULDER
#define geG_START XINPUT_GAMEPAD_START
#define geG_BACK XINPUT_GAMEPAD_BACK
#define geG_L_TRIGGER 69
#define geG_R_TRIGGER 420
#define geG_L_THUMBSTICK XINPUT_GAMEPAD_LEFT_THUMB
#define geG_R_THUMBSTICK XINPUT_GAMEPAD_RIGHT_THUMB
#define geG_DPAD_UP XINPUT_GAMEPAD_DPAD_UP
#define geG_DPAD_RIGHT XINPUT_GAMEPAD_DPAD_RIGHT
#define geG_DPAD_DOWN XINPUT_GAMEPAD_DPAD_DOWN
#define geG_DPAD_LEFT XINPUT_GAMEPAD_DPAD_LEFT

#define GAME_GAMEPAD_BATTERY_LOW BATTERY_LEVEL_FULL


	class GamePad
	{
	public:
		GamePad();
		bool wasButtonPressed(const uint32_t button, const uint32_t pad);
		bool wasButtonReleased(const uint32_t button, const uint32_t pad);
		bool isButtonHeld(const uint32_t button, const uint32_t pad);
		Vector2f PositionOf(const uint32_t analog, const uint32_t pad);
		void SetRumble(const Vector2f amount, const uint32_t pad);
		uint32_t BatteryLevel(const uint32_t pad);
		void Update();
		int32_t Id(const uint32_t pad) const;


		//XINPUT_GAMEPAD_TRIGGER_THRESHOLD 
		//Left thumbstick x-axis value. Each of the thumbstick axis members 
		// is a signed value between -32768 and 32767 describing the position of the thumbstick. 
		// A value of 0 is centered. Negative values signify down or to the left. Positive values 
		// signify up or to the right. The constants XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE or 
		// XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE can be used as a positive and negative value to filter a thumbstick input.
		//
		// triggers The value is between 0 and 255.
		 
	private:
		struct _PadState
		{
			// this needs to be all one structure
			XINPUT_STATE currentState;
			XINPUT_STATE oldState;

			bool isConnected;
			bool wasConnected;
			int32_t id;
			_PadState()
			{
				isConnected = false;
				wasConnected = false;
				id = -1;
				ZeroMemory(&currentState, sizeof(XINPUT_STATE));
				ZeroMemory(&oldState, sizeof(XINPUT_STATE));
			}
		};

		_PadState _padState[XUSER_MAX_COUNT];
	};

	inline uint32_t GamePad::BatteryLevel(const uint32_t pad)
	{
		XINPUT_BATTERY_INFORMATION info = {};
		XInputGetBatteryInformation(pad, BATTERY_DEVTYPE_GAMEPAD, &info);
		return info.BatteryLevel;
	}

	inline GamePad::GamePad()
	{
	}

	inline void GamePad::Update()
	{
		XINPUT_STATE test;

		for (uint32_t pad = 0; pad < XUSER_MAX_COUNT; pad++)
		{
			ZeroMemory(&test, sizeof(XINPUT_STATE));

			if (XInputGetState(pad, &test) == ERROR_SUCCESS)
			{
				// check for pre connected controllers here ??

				_padState[pad].id = pad;
				if (_padState[pad].isConnected)
				{
					_padState[pad].wasConnected = true;
				}
				else
				{
					_padState[pad].wasConnected = false;
				}
				_padState[pad].isConnected = true;
			}
			else
			{
				_padState[pad].id = -1;
				ZeroMemory(&_padState[pad].currentState, sizeof(XINPUT_STATE));
				ZeroMemory(&_padState[pad].oldState, sizeof(XINPUT_STATE));
				if (_padState[pad].isConnected)
				{
					_padState[pad].wasConnected = true;
				}
				else
				{
					_padState[pad].wasConnected = false;
				}
				_padState[pad].isConnected = false;

			}		

			if (_padState[pad].isConnected)
			{
				_padState[pad].oldState = _padState[pad].currentState;
				_padState[pad].currentState = test;
			}


		}
	}

	inline int32_t GamePad::Id(const uint32_t pad) const
	{
		return _padState[pad].id;
	}

	inline bool GamePad::wasButtonPressed(const uint32_t button, const uint32_t pad)
	{

		return false;
	}

	inline bool GamePad::wasButtonReleased(const uint32_t button, const uint32_t pad)
	{

		return false;
	}

	inline bool GamePad::isButtonHeld(const uint32_t button, const uint32_t pad)
	{

		return false;
	}

	inline Vector2f GamePad::PositionOf(const uint32_t analog, const uint32_t pad)
	{
		Vector2f normalizedPosition(0, 0);

		if (!_padState[pad].isConnected) return normalizedPosition;

		switch (analog)
		{
		case geG_L_TRIGGER:
			normalizedPosition.x = _padState[pad].currentState.Gamepad.bLeftTrigger / 255.0f;
			normalizedPosition.y = normalizedPosition.x;
			break;
		case geG_R_TRIGGER:
			normalizedPosition.x = _padState[pad].currentState.Gamepad.bRightTrigger / 255.0f;
			normalizedPosition.y = normalizedPosition.x;
		case geG_L_THUMBSTICK:
			normalizedPosition.x = _padState[pad].currentState.Gamepad.sThumbLX / 32767.0f;
			normalizedPosition.y = _padState[pad].currentState.Gamepad.sThumbLY / 32767.0f;
		default:
			break;
		}


		return normalizedPosition;
	}

	inline void GamePad::SetRumble(const Vector2f amount, const uint32_t pad)
	{

	}
}


#endif