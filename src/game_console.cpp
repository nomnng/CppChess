#include "game_console.h"
#include "piece_movement.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>

static void enable_vt_processing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    if (!(mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
	    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	    SetConsoleMode(hOut, mode);
    }
}
#endif

static const char* WHITE_SQUARE_COLOR_CODE = "\033[42m";
static const char* BLACK_SQUARE_COLOR_CODE = "\033[43m";

static const char* WHITE_PIECE_COLOR_CODE = "\033[1;37m";
static const char* BLACK_PIECE_COLOR_CODE = "\033[30m";

static const char* COORDINATE_COLOR_CODE = "\033[36m";
static const char* RESET_COLOR_CODE = "\033[0m";

Game::Game() {
#ifdef _WIN32
	// virtual terminal processing needs to be enabled on windows 
	// if it's turned off console won't process color changing codes
	enable_vt_processing();
#endif
}

void Game::run() {
	std::cout << "Starting the game!" << std::endl;
	print_board();
	while (1) {
		std::string user_input;
		std::cout << "Your move: "; 
		std::getline(std::cin, user_input);

		if (user_input == "q") {
			std::cout << "Closing the game..." << std::endl;
			return;
		} else {
			process_user_move(user_input);
		}
	}
}

int32_t Game::parse_move(std::string &coords) {
	char letter = std::tolower(coords[0]);
	if (letter < 'a' || letter > 'h') {
		return -1;
	}

	char number = coords[1];
	if (number < '1' || number > '8') {
		return -1;
	}

	uint8_t column = letter - 'a';
	uint8_t row = number - '1';
	row = 7 - row; // square numeration starts from left upper corner of the board

	return row * board.get_width() + column;
}

void Game::process_user_move(std::string &move) {
	if (move.length() < 5) {
		std::cout << "Please provide coordinates following this example: \"d2 d4\"" << std::endl;
		return;
	}

	std::string first_move_coords = move.substr(0, 2);
	int32_t first_square_index = parse_move(first_move_coords);
	if (first_square_index < 0) {
		std::cout << "\"" << first_move_coords << "\" is not square coordinate!" << std::endl;
		return;
	}

	std::string second_move_coords = move.substr(3, 2);
	int32_t second_square_index = parse_move(second_move_coords);
	if (second_square_index < 0) {
		std::cout << "\"" << second_move_coords << "\" is not square coordinate!" << std::endl;
		return;
	}

	PieceMovement piece_movement(board, first_square_index);
	if (!piece_movement.is_move_possible(second_square_index)) {
		std::cout << "PieceMovement::is_move_possible FAILED!" << std::endl;
	} else {
		MoveResult result = board.make_move(first_square_index, second_square_index);
		if (result == MoveResult::Incorrect) {
			std::cout << "Incorrect move! Follow the rules!" << std::endl;
			return;
		}		
	}

	// if (!PieceMovement::is_move_possible(board, first_square_index, second_square_index)) {
	// 	std::cout << "PieceMovement::is_move_possible FAILED!" << std::endl;
	// }


	// std::cout << "VALID MOVE, SQUARE: "
	// 	<< std::to_string(first_square_index) << ", "
	// 	<< std::to_string(second_square_index) << std::endl;

	print_board();
};

void Game::print_board() {
	std::cout << std::endl;
	for (uint32_t row = 0; row < board.get_width(); row++) {
		std::cout << COORDINATE_COLOR_CODE << std::to_string(8 - row) << " ";
		for (uint32_t column = 0; column < board.get_height(); column++) {
			char piece_symbol = ' ';
			Piece::Type piece = board.get_square(row, column);
			if (Piece::is_pawn(piece)) {
				piece_symbol = 'P';
			} else if (Piece::is_rook(piece)) {
				piece_symbol = 'R';
			} else if (Piece::is_knight(piece)) {
				piece_symbol = 'N';
			} else if (Piece::is_bishop(piece)) {
				piece_symbol = 'B';
			} else if (Piece::is_queen(piece)) {
				piece_symbol = 'Q';
			} else if (Piece::is_king(piece)) {
				piece_symbol = 'K';
			}

			std::string square_color_code = ((row + column) % 2) ? WHITE_SQUARE_COLOR_CODE : BLACK_SQUARE_COLOR_CODE;
			std::string piece_color_code = Piece::is_white(piece) ? WHITE_PIECE_COLOR_CODE : BLACK_PIECE_COLOR_CODE;
			std::cout << square_color_code << piece_color_code << piece_symbol << " " << RESET_COLOR_CODE;
		}
		std::cout << std::endl;
	}
	std::cout << COORDINATE_COLOR_CODE << "  a b c d e f g h" << RESET_COLOR_CODE << std::endl;

}
