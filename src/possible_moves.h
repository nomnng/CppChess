#include <memory>

#include "board.h"

class PossibleMoves {
public:
	PossibleMoves(uint32_t board_width, uint32_t board_height);
	bool set_possible_if_in_bounds(uint32_t row, uint32_t column);
	bool set_possible_if(const Board& board, uint32_t row, uint32_t column, Piece::Type piece);
	bool set_possible_if_not(const Board& board, uint32_t row, uint32_t column, Piece::Type piece);
	bool set_possible_if_enemy(const Board& board, uint32_t row, uint32_t column, bool is_white);
	void set_possible(uint32_t row, uint32_t column);
	void set_impossible(uint32_t row, uint32_t column);
	void set_possible(uint32_t square_index);
	void set_impossible(uint32_t square_index);

	bool is_possible(uint32_t row, uint32_t column) const { return possible_moves[row * width + column]; };
	bool is_possible(uint32_t square_index) const { return possible_moves[square_index]; };

	void combine(const PossibleMoves& possible_moves_obj);
	void subtract(const PossibleMoves& possible_moves_obj);
	
private:
	const uint32_t width;
	const uint32_t height;

	std::unique_ptr<bool[]> possible_moves;

};