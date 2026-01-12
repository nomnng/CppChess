#include "board.h"

Board::Board()
	: width(8), height(8), square_count(8 * 8), internal_state(8)
{
	board = std::make_unique<Piece::Type[]>(square_count);
	setup_default_board();
}

Board::Board(uint32_t board_width, uint32_t board_height)
	: width(board_width), height(board_height),
	square_count(board_width * board_height), internal_state(board_width)
{
	board = std::make_unique<Piece::Type[]>(square_count);

	for (uint32_t i = 0; i < square_count; i++) {
		board[i] = Piece::Type::None;
	}
}

bool Board::is_in_bounds(uint32_t row, uint32_t column) const {
	return row >= 0 && column >= 0 && row < height && column < width;
};

bool Board::is_square_equals(uint32_t row, uint32_t column, Piece::Type piece) const {
	if (!is_in_bounds(row, column)) {
		return false;
	}
	return board[row * width + column] == piece;
}

bool Board::is_square_not_equals(uint32_t row, uint32_t column, Piece::Type piece) const {
	if (!is_in_bounds(row, column)) {
		return false;
	}
	return board[row * width + column] != piece;
}

void Board::setup_default_board() {
	for (uint32_t row = 0; row < height; row++) {
		for (uint32_t column = 0; column < width; column++) {
			uint32_t i = row * width + column;

			Piece::Type piece = Piece::Type::None;

			if (row == 0) {
				constexpr Piece::Type black_pieces[] = {
					Piece::Type::BlackRook,
					Piece::Type::BlackKnight,
					Piece::Type::BlackBishop,
					Piece::Type::BlackQueen,
					Piece::Type::BlackKing,
					Piece::Type::BlackBishop,
					Piece::Type::BlackKnight,
					Piece::Type::BlackRook
				};
				piece = black_pieces[column];
			} else if (row == 1) {
				piece = Piece::Type::BlackPawn;
			} else if (row == 6) {
				piece = Piece::Type::WhitePawn;
			} else if (row == 7) {
				constexpr Piece::Type white_pieces[] = {
					Piece::Type::WhiteRook,
					Piece::Type::WhiteKnight,
					Piece::Type::WhiteBishop,
					Piece::Type::WhiteQueen,
					Piece::Type::WhiteKing,
					Piece::Type::WhiteBishop,
					Piece::Type::WhiteKnight,
					Piece::Type::WhiteRook
				};
				piece = white_pieces[column];
			}
			
			board[i] = piece;
		}
	}
}

void Board::make_move(uint32_t target_square_index, uint32_t destination_square_index) {
	Piece::Type piece = board[target_square_index];

	if (Piece::is_pawn(piece)) {
		uint32_t previous_en_passant_square_index = state.get_en_passant_square_index();
		int square_index_diff = previous_en_passant_square_index - target_square_index;
		if (square_index_diff == 1 || square_index_diff == -1) {
			board[previous_en_passant_square_index] = Piece::Type::None;
		}

		uint32_t target_row = get_row_by_square_index(target_square_index);
		uint32_t destination_row = get_row_by_square_index(destination_square_index);
		int diff = target_row - destination_row;
		if (diff == 2 || diff == -2) {
			internal_state.set_en_passant_square_index(destination_square_index);
		}
	} else if (Piece::is_rook(piece)) {
		internal_state.disable_castling();
	} else if (Piece::is_king(piece)) {
		uint32_t target_column = get_column_by_square_index(target_square_index);
		uint32_t destination_column = get_column_by_square_index(destination_square_index);
		int diff = target_column - destination_column;
		if ((diff == -2 || diff == 2)) {
			uint32_t rook_row = get_row_by_square_index(target_square_index);
			uint32_t rook_column = diff > 0 ? 0 : (width - 1);
			uint32_t rook_square_index = get_square_index(rook_row, rook_column);

			int rook_direction = diff > 0 ? 1 : -1;
			uint32_t new_rook_square_index = get_square_index(rook_row, destination_column + rook_direction);

			board[new_rook_square_index] = board[rook_square_index];
			board[rook_square_index] = Piece::Type::None;

		}
		internal_state.disable_castling();
	}

	board[destination_square_index] = piece;
	board[target_square_index] = Piece::Type::None;
	internal_state.change_turn();
}

