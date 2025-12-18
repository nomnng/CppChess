#pragma once

#include <stdint.h>

namespace Piece {
	constexpr int COLOR_BIT = 4; // this bit will decide the color of the piece 

	enum class Type : uint8_t {
		None = 0,

		WhitePawn = 1,
		WhiteKnight,
		WhiteBishop,
		WhiteRook,
		WhiteQueen,
		WhiteKing,

		BlackPawn = 1 << COLOR_BIT,
		BlackKnight,
		BlackBishop,
		BlackRook,
		BlackQueen,
		BlackKing,
	};

	inline bool is_pawn(Type p) {
		return p == Type::WhitePawn || p == Type::BlackPawn;
	}

	inline bool is_knight(Type p) {
		return p == Type::WhiteKnight || p == Type::BlackKnight;
	}

	inline bool is_bishop(Type p) {
		return p == Type::WhiteBishop || p == Type::BlackBishop;	
	}

	inline bool is_rook(Type p) {
		return p == Type::WhiteRook || p == Type::BlackRook;	
	}

	inline bool is_queen(Type p) {
		return p == Type::WhiteQueen || p == Type::BlackQueen;	
	}

	inline bool is_king(Type p) {
		return p == Type::WhiteKing || p == Type::BlackKing;	
	}

	inline bool is_black(Type p) {
		return (static_cast<uint8_t>(p) & (1 << COLOR_BIT));
	}

	inline bool is_white(Type p) {
		return !is_black(p);
	}
};


