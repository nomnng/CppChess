#include "piece_movement.h"
#include <iostream>

PieceMovement::PieceMovement(const Board& board, uint32_t target_square)
	: board(board), target_piece(board.get_square(target_square))
{
	target_possible_moves = calculate_possible_moves(target_square);
}

std::unique_ptr<PossibleMoves> PieceMovement::calculate_possible_moves(uint32_t target_square) {
	Piece::Type piece = board.get_square(target_square);
	uint32_t row = board.get_row_by_square_index(target_square);
	uint32_t column = board.get_column_by_square_index(target_square);

	if (Piece::is_pawn(piece)) {
		return get_pawn_moves(row, column);
	} else if (Piece::is_knight(piece)) {
		return get_knight_moves(row, column);
	} else if (Piece::is_rook(piece)) {
		return get_rook_moves(row, column);
	} else if (Piece::is_bishop(piece)) {
		return get_bishop_moves(row, column);
	} else if (Piece::is_queen(piece)) {
		return get_queen_moves(row, column);
	} else if (Piece::is_king(piece)) {
		return get_king_moves(row, column);
	}
}

std::unique_ptr<PossibleMoves> PieceMovement::get_pawn_moves(uint32_t row, uint32_t column) {
	std::unique_ptr<PossibleMoves> possible_moves = std::make_unique<PossibleMoves>(
		board.get_width(), board.get_height()
	);

	Piece::Type piece = board.get_square(row, column);
	bool is_white = Piece::is_white(piece);
	int direction = is_white ? -1 : 1;

	if (possible_moves->set_possible_if(board, row + direction, column, Piece::Type::None)) {
		// two square move
		if (is_white && row == (board.get_height() - 2)) {
			possible_moves->set_possible_if(board, row + direction * 2, column, Piece::Type::None);
		} else if (!is_white && row == 1) {
			possible_moves->set_possible_if(board, row + direction * 2, column, Piece::Type::None);
		}
	}

	uint32_t capture_row = row + direction;
	int offsets[] = {1, -1};
	for (int offset : offsets) {
		uint32_t capture_column = column + offset;
		if (!possible_moves->set_possible_if_not(board, capture_row, capture_column, Piece::Type::None)) {
			uint32_t square_index = board.get_square_index(row, capture_column);
			if (square_index == board.state.get_en_passant_square_index()) {
				possible_moves->set_possible(capture_row, capture_column);
			}
		}
	}
	return possible_moves;
}

std::unique_ptr<PossibleMoves> PieceMovement::get_pawn_attacked_squares(uint32_t row, uint32_t column) {
	std::unique_ptr<PossibleMoves> possible_moves = std::make_unique<PossibleMoves>(
		board.get_width(), board.get_height()
	);

	Piece::Type piece = board.get_square(row, column);
	bool is_white = Piece::is_white(piece);
	int direction = is_white ? -1 : 1;

	uint32_t capture_row = row + direction;
	int offsets[] = {1, -1};
	for (int offset : offsets) {
		uint32_t capture_column = column + offset;
		possible_moves->set_possible_if_in_bounds(capture_row, capture_column);
	}
	return possible_moves;
}

std::unique_ptr<PossibleMoves> PieceMovement::get_knight_moves(uint32_t row, uint32_t column) {
	std::unique_ptr<PossibleMoves> possible_moves = std::make_unique<PossibleMoves>(
		board.get_width(), board.get_height()
	);

	int offsets[8][2] = {
		{1, 2}, {1, -2}, {2, 1}, {2, -1}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}
	};

	for (auto [offset_r, offset_c] : offsets) {
		uint32_t r = row + offset_r;
		uint32_t c = column + offset_c;

		possible_moves->set_possible_if_in_bounds(r, c);
	}
	return possible_moves;
}

std::unique_ptr<PossibleMoves> PieceMovement::get_rook_moves(uint32_t row, uint32_t column) {
	std::unique_ptr<PossibleMoves> possible_moves = std::make_unique<PossibleMoves>(
		board.get_width(), board.get_height()
	);

	int directions[4][2] = {
		{1, 0}, {0, 1}, {-1, 0}, {0, -1}
	}; 

	for (auto [direction_r, direction_c] : directions) {
		uint32_t r = row + direction_r;
		uint32_t c = column + direction_c;
		while (possible_moves->set_possible_if(board, r, c, Piece::Type::None)) {
			r += direction_r;
			c += direction_c;
		};
		possible_moves->set_possible_if_in_bounds(r, c);
	}
	return possible_moves;
}

std::unique_ptr<PossibleMoves> PieceMovement::get_bishop_moves(uint32_t row, uint32_t column) {
	std::unique_ptr<PossibleMoves> possible_moves = std::make_unique<PossibleMoves>(
		board.get_width(), board.get_height()
	);

	int directions[4][2] = {
		{1, 1}, {-1, 1}, {-1, -1}, {1, -1}
	}; 

	for (auto [direction_r, direction_c] : directions) {
		uint32_t r = row + direction_r;
		uint32_t c = column + direction_c;

		while (possible_moves->set_possible_if(board, r, c, Piece::Type::None)) {
			r += direction_r;
			c += direction_c;
		};
		possible_moves->set_possible_if_in_bounds(r, c);
	}
	return possible_moves;
}

std::unique_ptr<PossibleMoves> PieceMovement::get_queen_moves(uint32_t row, uint32_t column) {
	auto rook_possible_moves = get_rook_moves(row, column);
	auto bishop_possible_moves = get_bishop_moves(row, column);
	rook_possible_moves->combine(*bishop_possible_moves);
	return rook_possible_moves;
}

std::unique_ptr<PossibleMoves> PieceMovement::get_king_base_moves(uint32_t row, uint32_t column) {
	std::unique_ptr<PossibleMoves> possible_moves = std::make_unique<PossibleMoves>(
		board.get_width(), board.get_height()
	);

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) {
				continue;
			}

			uint32_t r = row + i;
			uint32_t c = column + j;
			possible_moves->set_possible_if_in_bounds(r, c);
		}
	}
	return possible_moves;
}

std::unique_ptr<PossibleMoves> PieceMovement::get_king_moves(uint32_t row, uint32_t column) {
	std::unique_ptr<PossibleMoves> possible_moves = get_king_base_moves(row, column);

	bool is_king_white = Piece::is_white(board.get_square(row, column));

	for (uint32_t square_index = 0; square_index < board.get_square_count(); square_index++) {
		Piece::Type piece = board.get_square(square_index);
		if (piece == Piece::Type::None) {
			continue;
		}

		if (Piece::is_white(piece) != is_king_white) {
			if (Piece::is_king(piece)) {
				uint32_t r = board.get_row_by_square_index(square_index);
				uint32_t c = board.get_column_by_square_index(square_index);
				std::unique_ptr<PossibleMoves> king_moves = get_king_base_moves(r, c);
				possible_moves->subtract(*king_moves);
			} else if (Piece::is_pawn(piece)) {
				uint32_t r = board.get_row_by_square_index(square_index);
				uint32_t c = board.get_column_by_square_index(square_index);
				std::unique_ptr<PossibleMoves> pawn_moves = get_pawn_attacked_squares(r, c);
				possible_moves->subtract(*pawn_moves);
			} else {
				std::unique_ptr<PossibleMoves> piece_attacked_squares = calculate_possible_moves(square_index);
				possible_moves->subtract(*piece_attacked_squares);
			}
		}
	}

	return possible_moves;
}

bool PieceMovement::is_move_possible(uint32_t destination_square) {
	Piece::Type destination_piece = board.get_square(destination_square);
	if (destination_piece != Piece::Type::None) {
		if (Piece::is_white(destination_piece) == Piece::is_white(target_piece)) {
			return false;
		}
	}

	return target_possible_moves->is_possible(destination_square);
}
