#pragma once

#include <cstdint>
#include <memory>

#include "piece.h"
#include "board_state.h"

class Board {
private:
	BoardState internal_state;
public:
	const BoardState& state = internal_state;

	Board();
	Board(uint32_t board_width, uint32_t board_height);
	Piece::Type get_square(uint32_t row, uint32_t column) const { return board[row * width + column]; };
	Piece::Type get_square(uint32_t square_index) const { return board[square_index]; };
	void setup_default_board();
	uint32_t get_width() const { return width; };
	uint32_t get_height() const { return height; };
	uint32_t get_square_count() const { return square_count; };
	uint32_t get_row_by_square_index(uint32_t index) const { return index / width; };
	uint32_t get_column_by_square_index(uint32_t index) const { return index % width; };
	uint32_t get_square_index(uint32_t row, uint32_t column) const { return row * width + column; };
	void make_move(uint32_t target_square_index, uint32_t destination_square_index);
	bool is_in_bounds(uint32_t row, uint32_t column) const;
	bool is_square_equals(uint32_t row, uint32_t column, Piece::Type piece) const;
	bool is_square_not_equals(uint32_t row, uint32_t column, Piece::Type piece) const;
	
private:
	const uint32_t width;
	const uint32_t height;
	const uint32_t square_count;

	std::unique_ptr<Piece::Type[]> board;
};