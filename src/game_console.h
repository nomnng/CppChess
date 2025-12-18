#include "board.h"

class Game {
public:
	Game();
	void print_board();
	void run();

private:
	Board board;

	void process_user_move(std::string &move);
	int32_t parse_move(std::string &coords);
};