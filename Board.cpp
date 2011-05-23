#include "stdafx.h"

#include "Includes\Board.h"
#include "Includes\common.h"


//
// Setup the initial board positions and game state
//
board::board()
{
	Board[WHITE].setTeam(WHITE);	// Set the teams and move the pieces to
	Board[BLACK].setTeam(BLACK);	//	default positions
	BoardOverview = Board[WHITE].Board | Board[BLACK].Board;
	CurrentPlayer = WHITE;			// First player is always white
	HasCastled[WHITE] = false;
	HasCastled[BLACK] = false;
	UndoingMove = false;
}


board::~board()
{

}


inline unsigned int board::NextPlayer()
{
	//
	// Looks at the current player and returns the next player
	//	to move.
	//

	if (CurrentPlayer == WHITE)
		return BLACK;
	else
		return WHITE;
}


bool board::ApplyMove(move &Move)
{
	//
	// Uses the information contained in the 'move' structure
	//	to apply a move on the chess board.
	//	No checks are made to confirm moves are valid as all we
	//	are concerned with is board representation and board
	//	weights here.
	//

	bool noError = true;

	//
	// Check the piece we are moving exists
	//
	unsigned int SourcePiece = Board[CurrentPlayer].findPiece(Move.SourceSquare);
	if (SourcePiece == NOTHING)
		return false;

	//
	// Check we are not moving to a square with one of our own
	//	pieces on it.
	//
	unsigned int DestinationPiece = Board[CurrentPlayer].findPiece(Move.DestinationSquare);
	if (DestinationPiece != NOTHING)
		return false;

	//
	// If there is an enemy piece on the destination square then we
	//	remove it from the board.
	//
	DestinationPiece = Board[NextPlayer()].findPiece(Move.DestinationSquare);
	if (DestinationPiece != NOTHING)
		noError = Board[NextPlayer()].remPiece(DestinationPiece, Move.DestinationSquare);
	else if (Move.MoveType == MOVE_CAPTURE_EN_PASSANT)
	{
		//
		// Check we are not doing an en_passant move, if so calculate the
		//	square that contains the piece we want to remove, check its a
		//	pawn and remove it.
		//
		unsigned int Square;

		if (Move.DestinationSquare < 24)
			Square = Move.DestinationSquare + 8;
		else if (Move.DestinationSquare > 39)
			Square = Move.DestinationSquare - 8;
		else
			return false;

		DestinationPiece = Board[NextPlayer()].findPiece(Square);
		if (DestinationPiece != PAWN)
			return false;

		noError = Board[NextPlayer()].remPiece(DestinationPiece, Square);
	}
	Move.CapturedPiece = DestinationPiece;

	//
	// We apply the move of our piece on our board.
	//
	if (noError)
	{
		noError = Board[CurrentPlayer].applyMove(Move);

		//
		// If we are castling then we must also move the ROOK.
		//	Create a new move structure and recurse.
		//
		if (noError && ((Move.MoveType == MOVE_CASTLING_KINGSIDE) || (Move.MoveType == MOVE_CASTLING_QUEENSIDE)))
		{
			move Castle;

			Castle.MovingPiece = ROOK;
			Castle.RestingPiece = ROOK;
			Castle.MoveType = MOVE_NORMAL;

			switch (Move.MoveType)
			{
				case MOVE_CASTLING_KINGSIDE:
					Castle.DestinationSquare = Move.DestinationSquare - 1;
					Castle.SourceSquare = Move.SourceSquare + 3;
					break;
				case MOVE_CASTLING_QUEENSIDE:
					Castle.DestinationSquare = Move.DestinationSquare + 1;
					Castle.SourceSquare = Move.SourceSquare - 4;
					break;
			}

			noError = ApplyMove(Castle);
			CurrentPlayer = NextPlayer();	// Restore the current player
			HasCastled[CurrentPlayer] = true;
		}
		//
		// Mark moving pieces, such as king and rook as these effect castling.
		//
		else if((Move.MovingPiece == KING) && !UndoingMove)
		{
			if(((CurrentPlayer == WHITE) && (Move.SourceSquare == 4)) || ((CurrentPlayer == BLACK) && (Move.SourceSquare == 60)))
				Board[CurrentPlayer].KingMoved++;
		}
		else if((Move.MovingPiece == ROOK) && !UndoingMove)
		{
			//
			// Left
			//
			if(((CurrentPlayer == WHITE) && (Move.SourceSquare == 0)) || ((CurrentPlayer == BLACK) && (Move.SourceSquare == 56)))
				Board[CurrentPlayer].RookMoved[0]++;
			//
			// Right
			//
			else if(((CurrentPlayer == WHITE) && (Move.SourceSquare == 7)) || ((CurrentPlayer == BLACK) && (Move.SourceSquare == 63)))
				Board[CurrentPlayer].RookMoved[1]++;
		}
	}

	//
	// Change the player
	//
	CurrentPlayer = NextPlayer();


	//
	// Update the overview bitmap
	//
	BoardOverview = Board[WHITE].Board | Board[BLACK].Board;


	//
	// Return the error status.
	//	If there was an error in this function then the board
	//	status is unknown and something went terribly wrong
	//	somewhere else within the game.
	//
	return noError;
}


bool board::UndoMove(move Move)
{
	unsigned int Temp;
	UndoingMove = true;

	//
	// Reverse the move:
	//
	Temp = Move.DestinationSquare;
	Move.DestinationSquare = Move.SourceSquare;
	Move.SourceSquare = Temp;

	//
	// Reverse any promotions
	//
	Temp = Move.RestingPiece;
	Move.RestingPiece = Move.MovingPiece;
	Move.MovingPiece = Temp;

	//
	// Make note of the move type
	//	(So we can reverse special moves appropriately)
	//
	unsigned int MoveType = Move.MoveType;
	Move.MoveType = MOVE_NORMAL;

	//
	// Save the captured piece as ApplyMove changes this.
	//
	Temp = Move.CapturedPiece;
	CurrentPlayer = NextPlayer();
	bool noError = ApplyMove(Move);

	if (noError)
	{
		//
		// If the move type is En_Passant then we must restore the
		//	captured pawn.
		//
		if (MoveType == MOVE_CAPTURE_EN_PASSANT)
		{
			if (Move.SourceSquare < 24)
				Temp = Move.SourceSquare + 8;
			else if (Move.SourceSquare > 39)
				Temp = Move.SourceSquare - 8;
			else
				return false;

			if(!Board[CurrentPlayer].addPiece(PAWN, Temp))
				return false;
		}

		//
		// Restore the captured piece for all other move types.
		//
		else if (Temp != NOTHING)
		{
			if (!Board[CurrentPlayer].addPiece(Temp, Move.SourceSquare))
				return false;
		}

		//
		// If we are castling then we must also move the ROOK.
		//	Create a new move structure and apply the move.
		//
		else if ((MoveType == MOVE_CASTLING_KINGSIDE) || (MoveType == MOVE_CASTLING_QUEENSIDE))
		{
			Move.MovingPiece = ROOK;
			Move.RestingPiece = ROOK;
			Move.MoveType = MOVE_NORMAL;

			switch (MoveType)
			{
				case MOVE_CASTLING_KINGSIDE:
					Move.DestinationSquare = Move.DestinationSquare + 3;
					Move.SourceSquare = Move.SourceSquare - 1;
					break;
				case MOVE_CASTLING_QUEENSIDE:
					Move.DestinationSquare = Move.DestinationSquare - 4;
					Move.SourceSquare = Move.SourceSquare + 1;
					break;
			}


			CurrentPlayer = NextPlayer();	// Restore the player to the current player
			HasCastled[CurrentPlayer] = false;
			noError = ApplyMove(Move);		// ApplyMove will change it back.
		}
		//
		// Mark moving pieces, such as king and rook as these effect castling.
		//
		else if(Move.MovingPiece == KING)
		{
			if(((CurrentPlayer == BLACK) && (Move.DestinationSquare == 4)) || ((CurrentPlayer == WHITE) && (Move.DestinationSquare == 60)))
				Board[NextPlayer()].KingMoved--;
		}
		else if(Move.MovingPiece == ROOK)
		{
			//
			// Left
			//
			if(((CurrentPlayer == BLACK) && (Move.DestinationSquare == 0)) || ((CurrentPlayer == WHITE) && (Move.DestinationSquare == 56)))
				Board[NextPlayer()].RookMoved[0]--;
			//
			// Right
			//
			else if(((CurrentPlayer == BLACK) && (Move.DestinationSquare == 7)) || ((CurrentPlayer == WHITE) && (Move.DestinationSquare == 63)))
				Board[NextPlayer()].RookMoved[1]--;
		}

		//
		// Go back to the player who we just un-did's move.
		//
		CurrentPlayer = NextPlayer();

		//
		// Update the overview bitmap
		//
		BoardOverview = Board[WHITE].Board | Board[BLACK].Board;
	}

	UndoingMove = false;
	return noError;
}


bool board::AddPiece(unsigned int player, unsigned int piece, unsigned int square)
{
	bool success = Board[player].addPiece(piece, square);

	BoardOverview = Board[WHITE].Board | Board[BLACK].Board;

	return success;
}

void board::RemPiece(unsigned int square)
{
	unsigned int piece = Board[CurrentPlayer].findPiece(square);
	if(piece != NOTHING)
		Board[CurrentPlayer].remPiece(piece, square);
	else
	{
		piece = Board[NextPlayer()].findPiece(square);
		if(piece != NOTHING)
			Board[NextPlayer()].remPiece(piece, square);
	}

	BoardOverview = Board[WHITE].Board | Board[BLACK].Board;
}
