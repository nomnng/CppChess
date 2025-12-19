#include <memory>

#include "board.h"

class PossibleMoves {
public:
	PossibleMoves(uint32_t board_width, uint32_t board_height);
	bool set_possible_if(const Board& board, uint32_t row, uint32_t column, Piece::Type piece);
	bool set_possible_if_not(const Board& board, uint32_t row, uint32_t column, Piece::Type piece);
	bool set_possible_if_enemy(const Board& board, uint32_t row, uint32_t column, bool is_white);
	void set_possible(uint32_t row, uint32_t column);

	bool is_possible(uint32_t row, uint32_t column) const { return possible_moves[row * width + column]; };
	bool is_possible(uint32_t square_index) const { return possible_moves[square_index]; };

private:
	const uint32_t width;
	const uint32_t height;

	std::unique_ptr<bool[]> possible_moves;

};