#include "stdafx.h"

#include "includes\Side.h"
#include "includes\Common.h"


//
// An independant flag to indicate if we have initialised
//	the static lookup table in the class.
//	I did this because i couldn't be bothered manually typing
//	in all the values to have a constant static member SquareBits.
//
bool side::InitialisedSquares = false;
int64 side::SquareBits[64];


side::side()
{
	//
	// Check if our lookup table is built yet
	//

	if (!InitialisedSquares)
	{
		InitStaticVariables();
	}
}


void side::InitStaticVariables()
{
	//
	// Builds our lookup table,
	//	This is static as validator uses this lookup table too.
	//

	for (int i = 0; i < 64; i++)
		SquareBits[i] = ((int64)1 << i);

	InitialisedSquares = true;
}


void side::setTeam(unsigned int team)
{
	//
	// Clears the pieces and overview bitmap for the
	//	current players board.
	//
	Board = 0;
	for (int i = 0; i <= 6; i++)
		Pieces[i] = 0;

	//
	// Adds the initial pieces to the board for the team specified
	//
	if (team == WHITE)
	{
		addPiece(ROOK, 0);
		addPiece(KNIGHT, 1);
		addPiece(BISHOP, 2);
		addPiece(QUEEN, 3);
		addPiece(KING, 4);
		addPiece(BISHOP, 5);
		addPiece(KNIGHT, 6);
		addPiece(ROOK, 7);
		for(int i = 8; i < 16; i++)
		{
			addPiece(PAWN, i);
		}
	}
	else
	{
		for( int i = 48; i < 56; i++ )
		{
			addPiece(PAWN, i);
		}
		addPiece(ROOK, 56);
		addPiece(KNIGHT, 57);
		addPiece(BISHOP, 58);
		addPiece(QUEEN, 59);
		addPiece(KING, 60);
		addPiece(BISHOP, 61);
		addPiece(KNIGHT, 62);
		addPiece(ROOK, 63);
	}

	KingMoved = 0;
	RookMoved[0] = 0;	//Left
	RookMoved[1] = 0;	//Right
}


unsigned int side::findPiece(unsigned int Square)
{
	//
	// Searches for a piece at a certain location and
	//	returns the type of piece.
	//

	if (Square > 63)
		return NOTHING;

	if((Pieces[PAWN] & SquareBits[Square]) != 0)
		return PAWN;
	else if((Pieces[KNIGHT] & SquareBits[Square]) != 0)
		return KNIGHT;
	else if((Pieces[BISHOP] & SquareBits[Square]) != 0)
		return BISHOP;
	else if((Pieces[ROOK] & SquareBits[Square]) != 0)
		return ROOK;
	else if((Pieces[QUEEN] & SquareBits[Square]) != 0)
		return QUEEN;
	else if((Pieces[KING] & SquareBits[Square]) != 0)
		return KING;
	else
		return NOTHING;
}


bool side::addPiece(unsigned int Piece, unsigned int Square)
{
	//
	// Adds a piece to the board at a certain location
	//
	if (((Piece <= 6) && (Square < 64)))
	{
		Pieces[Piece] |= SquareBits[Square];
		Board |= SquareBits[Square];
		return true;
	}
	else
		return false;
}



bool side::remPiece(unsigned int Piece, unsigned int Square)
{
	//
	// Removes a piece from the board
	//
	if (!((Piece > 6) || (Square > 63)))
	{
		Pieces[Piece] ^= SquareBits[Square];
		Board ^= SquareBits[Square];
		return true;
	}
	else
		return false;
}


bool side::applyMove(move &Move)
{
	//
	// Applies a move by removing the piece specified
	//	and adding a piece at the destination (Allows for promotion)
	//
	bool noError = true;

	noError = remPiece(Move.MovingPiece, Move.SourceSquare);
	if (noError)
		noError = addPiece(Move.RestingPiece, Move.DestinationSquare);
	
	return noError;
}


