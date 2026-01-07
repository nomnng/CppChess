#include "board_state.h"

BoardState::BoardState(uint32_t board_width) {
}

void BoardState::change_turn() {
	white_turn_flag = !white_turn_flag;
	en_passant_square_index = new_en_passant_square_index;
	new_en_passant_square_index = -1;
}

void BoardState::disable_white_castling() {
	white_castling_possible = false;
};

void BoardState::disable_black_castling() {
	black_castling_possible = false;
};

void BoardState::set_en_passant_square_index(int32_t index) {
	new_en_passant_square_index = index;
}

bool BoardState::is_castling_possible() {
	if (white_turn_flag) {
		return white_castling_possible;
	} else {
		return black_castling_possible;		
	}
};

