#pragma once

#include <memory>
#include <cstdint>

class BoardState {
public:
	BoardState(uint32_t board_width);
	void disable_white_castling() { white_castling_possible = false; };
	void disable_black_castling() { black_castling_possible = false; };
	bool is_white_turn() const { return white_turn_flag; };
	bool is_black_turn() const { return !white_turn_flag; };
	void change_turn() { white_turn_flag = !white_turn_flag; };
	int32_t get_en_passant_square_index() const { return possible_en_passant_square_index; };
	void set_en_passant_square_index(int32_t index) { possible_en_passant_square_index = index; };
	bool is_castling_possible();

private:
	bool white_castling_possible = true;
	bool black_castling_possible = true;
	bool white_turn_flag = true;

	int32_t possible_en_passant_square_index = -1;
};