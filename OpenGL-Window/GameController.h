#if !defined(GAMECONTROLLER_H)
#define GAMECONTROLLER_H

#include <Xinput.h>

constexpr auto GAME_CONTROLLER_1 = 1;
constexpr auto GAME_CONTROLLER_2 = (1<<1);
constexpr auto GAME_CONTROLLER_3 = (1<<2);
constexpr auto GAME_CONTROLLER_4 = (1<<3);


namespace game
{
	// four controllers max
	class Controller
	{
	public:
		// use private struct here? to send back to user?
	private:
		// private struct
	};
}


#endif