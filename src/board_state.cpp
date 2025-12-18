#include "board_state.h"

BoardState::BoardState(uint32_t board_width) {
	// possible_en_passants_size = board_width;
	// possible_en_passants = std::make_unique<bool[]>(board_width);

	// moved_white_pawns = std::make_unique<bool[]>(board_width);
}

bool BoardState::is_castling_possible() {
	if (white_turn_flag) {
		return white_castling_possible;
	} else {
		return black_castling_possible;		
	}
};

