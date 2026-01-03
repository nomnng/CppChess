#include "possible_moves.h"

PossibleMoves::PossibleMoves(uint32_t board_width, uint32_t board_height)
	: width(board_width), height(board_height)
{
	possible_moves = std::make_unique<bool[]>(width * height);
}

bool PossibleMoves::set_possible_if_in_bounds(uint32_t row, uint32_t column) {
	if ((row >= 0 && row < height) && (column >= 0 && column < width)) {
		possible_moves[row * width + column] = true;
		return true;
	}
	return false;
}

void PossibleMoves::set_possible(uint32_t row, uint32_t column) {
	possible_moves[row * width + column] = true;
}

void PossibleMoves::set_possible(uint32_t square_index) {
	possible_moves[square_index] = true;
}

void PossibleMoves::set_impossible(uint32_t row, uint32_t column) {
	possible_moves[row * width + column] = false;
}

void PossibleMoves::set_impossible(uint32_t square_index) {
	possible_moves[square_index] = false;
}

bool PossibleMoves::set_possible_if(const Board& board, uint32_t row, uint32_t column, Piece::Type piece) {
	if (board.is_square_equals(row, column, piece)) {
		set_possible(row, column);
		return true;
	}
	return false;
}

bool PossibleMoves::set_possible_if_not(const Board& board, uint32_t row, uint32_t column, Piece::Type piece) {
	if (board.is_square_not_equals(row, column, piece)) {
		set_possible(row, column);
		return true;
	}
	return false;
}

bool PossibleMoves::set_possible_if_enemy(const Board& board, uint32_t row, uint32_t column, bool is_white) {
	Piece::Type piece = board.get_square(row, column);

	if (Piece::is_white(piece) != is_white) {
		set_possible(row, column);
		return true;
	}
	return false;	
}

void PossibleMoves::combine(const PossibleMoves& possible_moves_obj) {
	uint32_t board_size = width * height;
	for (uint32_t i = 0; i < board_size; i++) {
		possible_moves[i] = possible_moves[i] | possible_moves_obj.possible_moves[i];
	}
}

void PossibleMoves::subtract(const PossibleMoves& possible_moves_obj) {
	uint32_t board_size = width * height;
	for (uint32_t i = 0; i < board_size; i++) {
		possible_moves[i] = possible_moves[i] & ~possible_moves_obj.possible_moves[i];
	}
}
