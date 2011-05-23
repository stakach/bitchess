#ifndef __COMMON_H__
#define __COMMON_H__

//
// How we represent various parts of the game:
//
enum SIDES	{WHITE, BLACK};
enum PIECES	{NOTHING, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
enum MOVES	{MOVE_NORMAL, MOVE_CAPTURE_ORDINARY, MOVE_CAPTURE_EN_PASSANT, MOVE_CASTLING_KINGSIDE,
			MOVE_CASTLING_QUEENSIDE, MOVE_RESIGN, MOVE_STALEMATE};
enum PLAYER	{HUMAN, COMPUTER};


//
//	What is a 64bit integer on this platform?
//		Edit this when changing compiler or platform.
//
#define int64 unsigned long long


//
//	This structure represents a players move.
//		It is used throughout to test, process and make moves.
//
struct move
{
	unsigned int MoveType;
	unsigned int MovingPiece, RestingPiece;
	unsigned int CapturedPiece;
	unsigned int SourceSquare, DestinationSquare;
};


#endif
