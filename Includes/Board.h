#ifndef __BOARD_H__
#define __BOARD_H__

#include "common.h"
#include "Side.h"


class board
{
	public:
		board();
		~board();
		bool ApplyMove(move &Move);
		bool UndoMove(move Move);
		unsigned int NextPlayer();
		bool AddPiece(unsigned int player, unsigned int piece, unsigned int square);
		void RemPiece(unsigned int square);

		side Board[2];						// Both sides of the board
		int64 BoardOverview;				// A bitmap of all squares containing pieces
		unsigned int CurrentPlayer;			// Who's turn is it?
		bool HasCastled[2];
		bool UndoingMove;
};

#endif
