#pragma once

#include <memory>
#include <cstdint>

class BoardState {
public:
	BoardState(uint32_t board_width);
	bool is_white_turn() const { return white_turn_flag; };
	bool is_black_turn() const { return !white_turn_flag; };
	int32_t get_en_passant_square_index() const { return en_passant_square_index; };
	bool is_castling_possible();
	void disable_white_castling();
	void disable_black_castling();
	void change_turn();
	void set_en_passant_square_index(int32_t index);

private:
	bool white_castling_possible = true;
	bool black_castling_possible = true;
	bool white_turn_flag = true;

	int32_t en_passant_square_index = -1;
	int32_t new_en_passant_square_index = -1;
};