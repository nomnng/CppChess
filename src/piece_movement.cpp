#include "piece_movement.h"

PieceMovement::PieceMovement(const Board& board, uint32_t target_square)
	: possible_moves(board.get_width(), board.get_height()),
	target_piece(board.get_square(target_square)), is_target_piece_white(Piece::is_white(target_piece))
{
	calculate_possible_moves(board, target_square);
}

void PieceMovement::calculate_possible_moves(const Board& board, uint32_t target_square) {
	uint32_t row = board.get_row_by_square_index(target_square);
	uint32_t column = board.get_column_by_square_index(target_square);

	if (Piece::is_pawn(target_piece)) {
		set_pawn_possible_moves(board, row, column);
	} else if (Piece::is_knight(target_piece)) {
		set_knight_possible_moves(board, row, column);
	} else if (Piece::is_rook(target_piece)) {
		set_rook_possible_moves(board, row, column);
	} else if (Piece::is_bishop(target_piece)) {
		set_bishop_possible_moves(board, row, column);
	} else if (Piece::is_queen(target_piece)) {
		set_queen_possible_moves(board, row, column);
	} else if (Piece::is_king(target_piece)) {
		set_king_possible_moves(board, row, column);
	}
}

void PieceMovement::set_pawn_possible_moves(const Board &board, uint32_t row, uint32_t column) {
	int direction = is_target_piece_white ? -1 : 1;

	if (possible_moves.set_possible_if(board, row + direction, column, Piece::Type::None)) {
		// two square move
		if (is_target_piece_white && row == (board.get_height() - 2)) {
			possible_moves.set_possible_if(board, row + direction * 2, column, Piece::Type::None);
		} else if (!is_target_piece_white && row == 1) {
			possible_moves.set_possible_if(board, row + direction * 2, column, Piece::Type::None);
		}
	}

	uint32_t capture_row = row + direction;
	int offsets[] = {1, -1};
	for (int offset : offsets) {
		uint32_t capture_column = column + offset;
		if (board.is_square_not_equals(capture_row, capture_column, Piece::Type::None)) {
			possible_moves.set_possible_if_enemy(board, capture_row, capture_column, is_target_piece_white);
		}
	}
}

void PieceMovement::set_knight_possible_moves(const Board &board, uint32_t row, uint32_t column) {
	int offsets[8][2] = {
		{1, 2}, {1, -2}, {2, 1}, {2, -1}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}
	};

	for (auto [offset_r, offset_c] : offsets) {
		uint32_t r = row + offset_r;
		uint32_t c = column + offset_c;

		if (board.is_in_bounds(r, c)) {
			bool is_empty = possible_moves.set_possible_if(board, r, c, Piece::Type::None);
			if (!is_empty) {
				possible_moves.set_possible_if_enemy(board, r, c, is_target_piece_white);
			}
		}
	}
}

void PieceMovement::set_rook_possible_moves(const Board &board, uint32_t row, uint32_t column) {
	int directions[4][2] = {
		{1, 0}, {0, 1}, {-1, 0}, {0, -1}
	}; 

	for (auto [direction_r, direction_c] : directions) {
		uint32_t r = row + direction_r;
		uint32_t c = column + direction_c;
		while (board.is_in_bounds(r, c)) {
			if (!possible_moves.set_possible_if(board, r, c, Piece::Type::None)) {
				possible_moves.set_possible_if_enemy(board, r, c, is_target_piece_white);
				break;				
			}

			r += direction_r;
			c += direction_c;
		};
	}
}

void PieceMovement::set_bishop_possible_moves(const Board &board, uint32_t row, uint32_t column) {
	int directions[4][2] = {
		{1, 1}, {-1, 1}, {-1, -1}, {1, -1}
	}; 

	for (auto [direction_r, direction_c] : directions) {
		uint32_t r = row + direction_r;
		uint32_t c = column + direction_c;
		while (board.is_in_bounds(r, c)) {
			if (!possible_moves.set_possible_if(board, r, c, Piece::Type::None)) {
				possible_moves.set_possible_if_enemy(board, r, c, is_target_piece_white);
				break;				
			}

			r += direction_r;
			c += direction_c;
		};
	}
}

void PieceMovement::set_queen_possible_moves(const Board &board, uint32_t row, uint32_t column) {
	set_rook_possible_moves(board, row, column);
	set_bishop_possible_moves(board, row, column);
}

void PieceMovement::set_king_possible_moves(const Board &board, uint32_t row, uint32_t column) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) {
				continue;
			}

			uint32_t r = row + i;
			uint32_t c = column + j;
			if (!possible_moves.set_possible_if(board, r, c, Piece::Type::None)) {
				possible_moves.set_possible_if_enemy(board, r, c, is_target_piece_white);
			}
		}
	}
}

bool PieceMovement::is_move_possible(uint32_t destination_square) {
	return possible_moves.is_possible(destination_square);
}
