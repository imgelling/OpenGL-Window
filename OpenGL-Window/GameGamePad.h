#if !defined(GAMECONTROLLER_H)
#define GAMECONTROLLER_H

#include <stdint.h>
#include <Xinput.h>

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

#define geG_A 1
#define geG_B 2
#define geG_X 3
#define geG_Y 4
#define geG_LBUMPER 5
#define geG_RBUMBER 6
#define geG_START 7
#define geG_SELECT 8
#define geG_LTRIGGER 9
#define geG_RTRIGGER 10
#define geG_LTHUMBSTICK 11
#define geG_RTHUMBSTICK 12


	class GamePad
	{
	public:
		GamePad();
		bool wasPressed(const uint32_t button);
		bool wasReleased(const uint32_t button);
		bool isHeld(const uint32_t button);
		Vector2f PositionOf(const uint32_t analog);
		void SetRumble(const Vector2f amount);
		void Update();


		//XINPUT_GAMEPAD_TRIGGER_THRESHOLD 
		//Left thumbstick x-axis value. Each of the thumbstick axis members 
		// is a signed value between -32768 and 32767 describing the position of the thumbstick. 
		// A value of 0 is centered. Negative values signify down or to the left. Positive values 
		// signify up or to the right. The constants XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE or 
		// XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE can be used as a positive and negative value to filter a thumbstick input.
		//
		// triggers The value is between 0 and 255.
		 
	private:
		XINPUT_STATE _currentState[XUSER_MAX_COUNT];
		XINPUT_STATE _oldState[XUSER_MAX_COUNT];

		bool _isConnected;
		bool _wasConnected;

	};

	inline GamePad::GamePad()
	{
		_isConnected = false;
		_wasConnected = false;
	}

	inline void GamePad::Update()
	{

	}

	inline bool GamePad::wasPressed(const uint32_t button)
	{

	}

	inline bool GamePad::wasReleased(const uint32_t button)
	{

	}

	inline bool GamePad::isHeld(const uint32_t button)
	{

	}

	inline Vector2f GamePad::PositionOf(const uint32_t analog)
	{
		Vector2f normalizedPosition(0, 0);

		return normalizedPosition;
	}

	inline void GamePad::SetRumble(const Vector2f amount)
	{

	}
}


#endif