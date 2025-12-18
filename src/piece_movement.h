#include <memory>

#include "board.h"

class PieceMovement {
public:
	PieceMovement(const Board& board, uint32_t target_square);
	bool is_move_possible(uint32_t destination_square);

private:
	std::unique_ptr<bool[]> possible_moves;
	const uint32_t width;
	const uint32_t height;
	const Piece::Type target_piece;
	const bool is_target_piece_white;

	void calculate_possible_moves(const Board& board, uint32_t target_square);
	bool set_possible_if(const Board& board, uint32_t row, uint32_t column, Piece::Type piece);
	bool set_possible_if_not(const Board& board, uint32_t row, uint32_t column, Piece::Type piece);
	bool set_possible_if_enemy(const Board& board, uint32_t row, uint32_t column);

	void set_possible(uint32_t row, uint32_t column);

	void set_pawn_possible_moves(const Board &board, uint32_t row, uint32_t column);
	void set_knight_possible_moves(const Board &board, uint32_t row, uint32_t column);
	void set_rook_possible_moves(const Board &board, uint32_t row, uint32_t column);
	void set_bishop_possible_moves(const Board &board, uint32_t row, uint32_t column);
	void set_queen_possible_moves(const Board &board, uint32_t row, uint32_t column);
	void set_king_possible_moves(const Board &board, uint32_t row, uint32_t column);
};

