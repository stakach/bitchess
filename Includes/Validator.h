#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__


#include <string>
#include "Common.h"
#include "Board.h"
#include "Generator.h"


using std::string;


class validator
{
	public:
		validator();
		bool CheckMove(board Board, move Move, string &Message, move &LastMove);
		bool CheckStatus(board Board, string &Message, move &LastMove, bool inValidator = false);

	private:
		generator Generator;
		static int64 Moves[7][64];
		static int64 Rook[4][64];
		static int64 Bishop[4][64];
		static bool generationComplete;

		enum ROOKDIR	{LEFT, RIGHT, UP, DOWN};
		enum BISHOPDIR	{DEG45, DEG135, DEG225, DEG315};

		static void generateMoveBitmaps();
		static int64 genBlackPawn(unsigned int SourceSquare);
		static int64 genWhitePawn(unsigned int SourceSquare);
		static int64 genKnight(unsigned int SourceSquare);
		static int64 genBishop(unsigned int SourceSquare);
		static int64 genRook(unsigned int SourceSquare);
		static int64 genQueen(unsigned int SourceSquare);
		static int64 genKing(unsigned int SourceSquare);
		static int64 genBishopMoves(board &Board, unsigned int &SourceSquare);
		static int64 genRookMoves(board &Board, unsigned int &SourceSquare);
		static int64 genPawnMoves(board &Board, unsigned int &SourceSquare);

		friend class generator;
};


#endif


