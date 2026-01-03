#include <memory>

#include "board.h"
#include "possible_moves.h"

class PieceMovement {
public:
	PieceMovement(const Board& board, uint32_t target_square);
	bool is_move_possible(uint32_t destination_square);

private:
	std::unique_ptr<PossibleMoves> target_possible_moves;
	const Board& board;
	Piece::Type target_piece;

	std::unique_ptr<PossibleMoves> calculate_possible_moves(uint32_t target_square);

	std::unique_ptr<PossibleMoves> get_pawn_moves(uint32_t row, uint32_t column);
	std::unique_ptr<PossibleMoves> get_pawn_attacked_squares(uint32_t row, uint32_t column);
	std::unique_ptr<PossibleMoves> get_knight_moves(uint32_t row, uint32_t column);
	std::unique_ptr<PossibleMoves> get_rook_moves(uint32_t row, uint32_t column);
	std::unique_ptr<PossibleMoves> get_bishop_moves(uint32_t row, uint32_t column);
	std::unique_ptr<PossibleMoves> get_queen_moves(uint32_t row, uint32_t column);
	std::unique_ptr<PossibleMoves> get_king_base_moves(uint32_t row, uint32_t column);
	std::unique_ptr<PossibleMoves> get_king_moves(uint32_t row, uint32_t column);
};

