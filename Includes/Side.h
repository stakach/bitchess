#ifndef __SIDE_H__
#define __SIDE_H__

#include "Common.h"


class side
{
	public:
		side();
		side(unsigned int team)
			{setTeam(team);};
		void setTeam(unsigned int team);
		unsigned int findPiece(unsigned int Square);
		bool addPiece(unsigned int Piece, unsigned int Square);
		bool remPiece(unsigned int Piece, unsigned int Square);
		bool applyMove(move &Move);

		int64 Pieces[7];					// The Pieces on the board and their locations (0 is temp)
		int64 Board;						// Bitmap of either white or black pieces
		unsigned int KingMoved;
		unsigned int RookMoved[2];

	private:
		static bool InitialisedSquares;		// Has the lookup table been created?
		static int64 SquareBits[64];		// Lookup table to increase speed finding pieces

		static void InitStaticVariables();

		friend class validator;
};

#endif
