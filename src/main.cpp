#ifdef WINAPI_BUILD
#include "game_winapi.h"
#endif

#ifdef CONSOLE_BUILD
#include "game_console.h"
#endif

int main() {
	Game game = Game();
	game.run();

	return 0;
}