#include "board.h"

class Game {
public:
	Game();
	void print_board();
	void run();

private:
	Board board;
	bool is_game_running;

	void process_user_move(std::string &move);
	int32_t parse_move(std::string &coords);
};