#include "stdafx.h"

#include "includes\Validator.h"
#include "includes\Common.h"
#include "includes\Board.h"
#include <string>
#include <list>


using std::string;
using std::list;


int64 validator::Moves[7][64];
int64 validator::Rook[4][64];
int64 validator::Bishop[4][64];
bool validator::generationComplete = false;


validator::validator()
{
	if(!generationComplete)
		generateMoveBitmaps();
}


void validator::generateMoveBitmaps()
{
	//
	// Check the board position lookup table has been generated.
	//	The side constructor also checks for itself.
	//
	if(!side::InitialisedSquares)
		side::InitStaticVariables();

	//
	// Generate the moves for each of the squares on the board
	//	for each piece.
	//
	for(unsigned int SourceSquare = 0; SourceSquare < 64; SourceSquare++)
	{
		Moves[PAWN - BLACK][SourceSquare] = genBlackPawn(SourceSquare);	// For pawns, only generate attack moves
		Moves[PAWN - WHITE][SourceSquare] = genWhitePawn(SourceSquare);
		Moves[KNIGHT][SourceSquare] = genKnight(SourceSquare);
		Moves[BISHOP][SourceSquare] = genBishop(SourceSquare);
		Moves[ROOK][SourceSquare] = genRook(SourceSquare);
		Moves[QUEEN][SourceSquare] = genQueen(SourceSquare);
		Moves[KING][SourceSquare] = genKing(SourceSquare);
	}


	//
	// Mark the flag as complete.
	//
	generationComplete = true;
}


int64 validator::genBlackPawn(unsigned int SourceSquare)
{
	int64 MoveBitmap = 0;
	int file = SourceSquare % 8;

	//
	// Generate all possible attack moves by pawns comming from the black
	//	side of the board.
	//
	if(SourceSquare > 7)
	{
		if(!(file == 0))
			MoveBitmap |= side::SquareBits[SourceSquare - 7];
		if(!(file == 7))
			MoveBitmap |= side::SquareBits[SourceSquare - 9];
	}

	return MoveBitmap;
}


int64 validator::genWhitePawn(unsigned int SourceSquare)
{
	int64 MoveBitmap = 0;
	int file = SourceSquare % 8;

	//
	// Generate all possible attack moves by pawns comming from the white
	//	side of the board.
	//
	if(SourceSquare < 56)
	{
		if(!(file == 0))
			MoveBitmap |= side::SquareBits[SourceSquare + 7];
		if(!(file == 7))
			MoveBitmap |= side::SquareBits[SourceSquare + 9];
	}

	return MoveBitmap;
}


int64 validator::genKnight(unsigned int SourceSquare)
{
	//
	// The bitboards we are using for the move generation
	//	and the row and file is used to make sure the moves don't
	//	go over the boundaries of the board.
	//
	int64 MoveBitmap = 0;
	int64 pos = side::SquareBits[SourceSquare];

	int row = SourceSquare / 8;
	int file = SourceSquare % 8;

	//
	// A knight has up to 8 possible moves in the open.
	//	Here we use the row and file to check which of
	//	these moves are possible from the current position.
	//
	if (row > 0 && file > 1)
	{
		MoveBitmap |= (pos >> 10);
	}
	if (row > 1 && file > 0)
	{
		MoveBitmap |= (pos >> 17);
	}
	if (row > 0 && file < 6)
	{
		MoveBitmap |= (pos >> 6);
	}
	if (row > 1 && file < 7)
	{
		MoveBitmap |= (pos >> 15);
	}

	if (row < 7 && file < 6)
	{
		MoveBitmap |= (pos << 10);
	}
	if (row < 6 && file < 7)
	{
		MoveBitmap |= (pos << 17);
	}
	if (row < 7 && file > 1)
	{
		MoveBitmap |= (pos << 6);
	}
	if (row < 6 && file > 0)
	{
		MoveBitmap |= (pos << 15);
	}

	return MoveBitmap;
}


int64 validator::genBishop(unsigned int SourceSquare)
{
	int64 MoveBitmap = 0;

	int row = SourceSquare / 8;
	int file = SourceSquare % 8;

	//
	// Move diagonally up and right (45)
	//	We set the source square to one as the xor turns it off.
	//
	Bishop[DEG45][SourceSquare] = side::SquareBits[SourceSquare];
	for(int i = row, j = file; (i < 8) && (j < 8); i++, j++)
	{
		MoveBitmap ^= side::SquareBits[i * 8 + j];
		Bishop[DEG45][SourceSquare] ^= side::SquareBits[i * 8 + j];
	}

	//
	// Move diagonally up and left (135)
	//
	Bishop[DEG135][SourceSquare] = side::SquareBits[SourceSquare];
	for(int i = row, j = file; (i < 8) && (j >= 0); i++, j--)
	{
		MoveBitmap ^= side::SquareBits[i * 8 + j];
		Bishop[DEG135][SourceSquare] ^= side::SquareBits[i * 8 + j];
	}

	//
	// Move diagonally down and left (225)
	//
	Bishop[DEG225][SourceSquare] = side::SquareBits[SourceSquare];
	for(int i = row, j = file; (i >= 0) && (j >= 0); i--, j--)
	{
		MoveBitmap ^= side::SquareBits[i * 8 + j];
		Bishop[DEG225][SourceSquare] ^= side::SquareBits[i * 8 + j];
	}

	//
	// Move diagonally down and right (315)
	//
	Bishop[DEG315][SourceSquare] = side::SquareBits[SourceSquare];
	for(int i = row, j = file; (i >= 0) && (j < 8); i--, j++)
	{
		MoveBitmap ^= side::SquareBits[i * 8 + j];
		Bishop[DEG315][SourceSquare] ^= side::SquareBits[i * 8 + j];
	}

	return MoveBitmap;
}


int64 validator::genRook(unsigned int SourceSquare)
{
	int64 MoveBitmap = 0;

	int row = SourceSquare / 8;
	int file = SourceSquare % 8;

/*	Decided against this method as we need to get the components, up down right left
	//
	// The one loop runs accross and up at the same time
	//	as its xor'ed the source square is 0'ed.
	//
	for(int i = 0; i < 8; i++)
	{
		MoveBitmap ^= side::SquareBits[i * 8 + file];
		MoveBitmap ^= side::SquareBits[row * 8 + i];
	}
*/

	Rook[UP][SourceSquare] = side::SquareBits[SourceSquare];
	for(int i = row; i < 8; i++) //up
	{
		MoveBitmap ^= side::SquareBits[i * 8 + file];
		Rook[UP][SourceSquare] ^= side::SquareBits[i * 8 + file];
	}

	Rook[DOWN][SourceSquare] = side::SquareBits[SourceSquare];
	for(int i = row; i >= 0; i--) //down
	{
		MoveBitmap ^= side::SquareBits[i * 8 + file];
		Rook[DOWN][SourceSquare] ^= side::SquareBits[i * 8 + file];
	}

	Rook[RIGHT][SourceSquare] = side::SquareBits[SourceSquare];
	for(int i = file; i < 8; i++) //right
	{
		MoveBitmap ^= side::SquareBits[row * 8 + i];
		Rook[RIGHT][SourceSquare] ^= side::SquareBits[row * 8 + i];
	}

	Rook[LEFT][SourceSquare] = side::SquareBits[SourceSquare];
	for(int i = file; i >= 0; i--) //left
	{
		MoveBitmap ^= side::SquareBits[row * 8 + i];
		Rook[LEFT][SourceSquare] ^= side::SquareBits[row * 8 + i];
	}

	return MoveBitmap;
}


int64 validator::genQueen(unsigned int SourceSquare)
{
	int64 MoveBitmap = 0;

	//
	// The queen is a combination of both the bishop and the rook.
	//
	MoveBitmap = genBishop(SourceSquare);
	MoveBitmap |= genRook(SourceSquare);

	return MoveBitmap;
}


int64 validator::genKing(unsigned int SourceSquare)
{
	int64 MoveBitmap = 0;
	int64 pos = side::SquareBits[SourceSquare];

	int row = SourceSquare / 8;
	int file = SourceSquare % 8;

	if (file > 0)
	{
		MoveBitmap |= (pos >> 1);
	}
	if (file < 7)
	{
		MoveBitmap |= (pos << 1);
	}
	if (row > 0)
	{
		MoveBitmap |= (pos >> 8);
	}
	if (row < 7)
	{
		MoveBitmap |= (pos << 8);
	}
	if (row < 7 && file > 0)
	{
		MoveBitmap |= (pos << 7);
	}
	if (row < 7 && file < 7)
	{
		MoveBitmap |= (pos << 9);
	}
	if (row > 0 && file > 0)
	{
		MoveBitmap |= (pos >> 9);
	}
	if (row > 0 && file < 7)
	{
		MoveBitmap |= (pos >> 7);
	}

	return MoveBitmap;
}


bool validator::CheckMove(board Board, move Move, string &Message, move &LastMove)
{
	bool moveAccepted = false;
	unsigned int movingPiece = Board.Board[Board.CurrentPlayer].findPiece(Move.SourceSquare);

	//
	// Check piece exists
	//
	if(movingPiece != NOTHING)
	{
		//
		// Check move is to a valid square.
		//
		if(Move.DestinationSquare > 63)
		{
			Message = "Can't move here.";
			return false;
		}

		//
		// Check player isn't trying to take his own piece.
		//
		else if(Board.Board[Board.CurrentPlayer].findPiece(Move.DestinationSquare) != NOTHING)
		{
			Message = "Can't capture your own piece.";
			return false;
		}

		//
		// Switch to the move type that needs to be validated.
		//
		switch(Move.MoveType)
		{
			case MOVE_NORMAL:
			case MOVE_CAPTURE_ORDINARY:
				switch(movingPiece)
				{
					case PAWN:
						//
						// We generate the pawn move bitboard and test if the move can
						//	be made
						//
						if(side::SquareBits[Move.DestinationSquare] & genPawnMoves(Board, Move.SourceSquare))
							moveAccepted = true;
						break;
					case KNIGHT:
						//
						// Just uses the pre-generated bitboards to calculate if the move
						//	we are requesting can be made.
						//
						if(side::SquareBits[Move.DestinationSquare] & Moves[KNIGHT][Move.SourceSquare])
							moveAccepted = true;
						break;

					case BISHOP:
						//
						// We generate the bishop move bitboard and test if the move can
						//	be made
						//
						if(side::SquareBits[Move.DestinationSquare] & genBishopMoves(Board, Move.SourceSquare))
							moveAccepted = true;
						break;

					case ROOK:
						//
						// Same as bishop
						//
						if(side::SquareBits[Move.DestinationSquare] & genRookMoves(Board, Move.SourceSquare))
							moveAccepted = true;
						break;

					case QUEEN:
						//
						// The Queen is a combination of both the Rook and the Bishop moves.
						//
						if(side::SquareBits[Move.DestinationSquare] & (genRookMoves(Board, Move.SourceSquare) | genBishopMoves(Board, Move.SourceSquare)))
							moveAccepted = true;
						break;

					case KING:
						//
						// Just uses the pre-generated bitboards to calculate if the move
						//	we are requesting can be made.
						//
						if(side::SquareBits[Move.DestinationSquare] & Moves[KING][Move.SourceSquare])
							moveAccepted = true;
						//fall through end of select (break;)
				}
				break;
			case MOVE_CAPTURE_EN_PASSANT:
				if(Moves[PAWN - Board.CurrentPlayer][Move.SourceSquare] & side::SquareBits[Move.DestinationSquare])
				{
					unsigned int row = Move.SourceSquare / 8;

					if((((Board.CurrentPlayer == WHITE) && (row == 4)) || ((Board.CurrentPlayer == BLACK) && (row == 5))) && (LastMove.MovingPiece == PAWN))
					{
					
						unsigned int square;

						if (Move.DestinationSquare < 24)
						{
							row = Move.DestinationSquare + 8;
							square = Move.DestinationSquare - 8;
						}
						else //if (Move.DestinationSquare > 39) irrelivant
						{
							row = Move.DestinationSquare - 8;
							square = Move.DestinationSquare + 8;
						}

						if((LastMove.DestinationSquare == row) && (LastMove.SourceSquare == square))
							moveAccepted = true;
					}
				}
				break;

			case MOVE_CASTLING_KINGSIDE:
			case MOVE_CASTLING_QUEENSIDE:

				//
				// Check if we have castled before
				//
				if(!Board.HasCastled[Board.CurrentPlayer])
				{
					//
					// Check if any of the relevant pieces have moved.
					//
					if((Board.Board[Board.CurrentPlayer].KingMoved + Board.Board[Board.CurrentPlayer].RookMoved[RIGHT]) == 0)
					{
						unsigned int checkSquare = 0;

						//
						// Calculate if there are any pieces in the way.
						//
						if(Move.MoveType == MOVE_CASTLING_KINGSIDE)
							checkSquare = Move.DestinationSquare - 1;
						else
							checkSquare = Move.DestinationSquare + 1;

						if(!(Board.BoardOverview & (side::SquareBits[Move.DestinationSquare] | side::SquareBits[checkSquare])))
						{
							//
							// Add kings to the board on the squares the king castles through
							//
							Board.AddPiece(Board.CurrentPlayer, KING, checkSquare);
							Board.AddPiece(Board.CurrentPlayer, KING, Move.DestinationSquare);


							//
							// Check if the player is in check or trying to castle through check.
							//
							Board.CurrentPlayer = Board.NextPlayer();
							moveAccepted = !CheckStatus(Board, Message, LastMove, true);

							//
							// If the move is accepted then change the player back.
							//
							if(moveAccepted)
							{
								Board.RemPiece(checkSquare);
								Board.RemPiece(Move.DestinationSquare);
								Board.CurrentPlayer = Board.NextPlayer();
							}
							else
								Message = "You can't castle through check!";
						}
						else
							Message = "Can't castle with pieces between your King and Rook.";
					}
					else
						Message = "One or more of the pieces involved has been moved!";
				}
				else
					Message = "You have already castled!";
				break;

			default:
				Message = "Unknown move selected.";
				return false;
				//break; (fall through)
		}

		//
		// Give the user feedback
		//
		if((moveAccepted == false) && (Message == ""))
			Message = "Not a valid move!";
	}
	else
	{
		//
		// If the current player does not have a piece at the location given
		//	check if the other player does and give the appropriate feedback.
		//
		if(Board.Board[Board.NextPlayer()].findPiece(Move.SourceSquare) == NOTHING)
			Message = "No chess piece selected.";
		else
			Message = "It is the other players turn.";
		return false;
	}

	//
	// If the move was accepted we apply the move,
	//	check the status of the board at this position
	//	making sure the current player is not putting
	//	himself in check.
	//
	if(moveAccepted)
	{
		if(Board.ApplyMove(Move))
		{
			moveAccepted = !CheckStatus(Board, Message, LastMove, true);
			if(moveAccepted)
				LastMove = Move;
			else
				Message = "You can't move into check!";
		}
		else
		{
			//
			// If an error occured in makemove for a validated move we are
			//	in trouble.. Tell the user.
			//
			moveAccepted = false;
			Message = "Logic error. Try again (This should not happen).";
		}
	}

	return moveAccepted;
}


bool validator::CheckStatus(board Board, string &Message, move &LastMove, bool inValidator)
{
	int64 attackedSquares = 0;	// Squares that are being attacked by the enemy.

	//
	// If we are checking the status of a move change the player back.
	//	Otherwise we are validating a move for the current player.
	//
	if(!inValidator)
		Board.CurrentPlayer = Board.NextPlayer();

	//
	// Check every square on the board for enemy pieces.
	//	If a piece is found, work out what piece it is 
	//
	for(unsigned int i = 0; i < 63; i++)
	{
		if(side::SquareBits[i] & Board.Board[Board.CurrentPlayer].Board)
		{
			unsigned int piece = Board.Board[Board.CurrentPlayer].findPiece(i);
			switch(piece)
			{
				case PAWN:
					attackedSquares |= (Moves[PAWN - Board.CurrentPlayer][i] & ~Board.Board[Board.CurrentPlayer].Board);
					break;

				case KNIGHT:
					attackedSquares |= (Moves[KNIGHT][i] & ~Board.Board[Board.CurrentPlayer].Board);
					break;

				case BISHOP:
					attackedSquares |= genBishopMoves(Board, i);
					break;

				case ROOK:
					attackedSquares |= genRookMoves(Board, i);
					break;

				case QUEEN:
					attackedSquares |= (genRookMoves(Board, i) | genBishopMoves(Board, i));
					break;

				case KING:
					attackedSquares |= (Moves[KING][i] & ~Board.Board[Board.CurrentPlayer].Board);
					//fall through end of select (break;)
			}
		}
	}

	//
	// If the attackedSquares is attacking the king then we can't make this move
	//	as we will be moving into check (or are in check if we are getting status)
	//
	bool inCheck = false;

	if(attackedSquares & Board.Board[Board.NextPlayer()].Pieces[KING])
	{
		//
		// If we are just checking status then set the message.
		//	Other wise inform the Validator.
		//
		if(!inValidator)
		{
			Message = "Check!";
		}
		else
			inCheck = true;
	}

	return inCheck;
}


int64 validator::genRookMoves(board &Board, unsigned int &SourceSquare)
{
	int64 right_moves = Rook[RIGHT][SourceSquare] & Board.BoardOverview;
	right_moves = (right_moves<<1) | (right_moves<<2) | (right_moves<<3) | (right_moves<<4) | (right_moves<<5) | (right_moves<<6);
	right_moves = right_moves & Rook[RIGHT][SourceSquare];
	right_moves = (right_moves ^ Rook[RIGHT][SourceSquare]) ^ (Board.Board[Board.NextPlayer()].Board & ~Board.BoardOverview);

	int64 left_moves = Rook[LEFT][SourceSquare] & Board.BoardOverview;
	left_moves = (left_moves>>1) | (left_moves>>2) | (left_moves>>3) | (left_moves>>4) | (left_moves>>5) | (left_moves>>6);
	left_moves = left_moves & Rook[LEFT][SourceSquare];
	left_moves = (left_moves ^ Rook[LEFT][SourceSquare]) ^ (Board.Board[Board.NextPlayer()].Board & ~Board.BoardOverview);

	int64 up_moves = Rook[UP][SourceSquare] & Board.BoardOverview;
	up_moves = (up_moves<<8) | (up_moves<<16) | (up_moves<<24) | (up_moves<<32) | (up_moves<<40) | (up_moves<<48);
	up_moves = up_moves & Rook[UP][SourceSquare];
	up_moves = (up_moves ^ Rook[UP][SourceSquare]) ^ (Board.Board[Board.NextPlayer()].Board & ~Board.BoardOverview);

	int64 down_moves = Rook[DOWN][SourceSquare] & Board.BoardOverview;
	down_moves = (down_moves>>8) | (down_moves>>16) | (down_moves>>24) | (down_moves>>32) | (down_moves>>40) | (down_moves>>48);
	down_moves = down_moves & Rook[DOWN][SourceSquare];
	down_moves = (down_moves ^ Rook[DOWN][SourceSquare]) ^ (Board.Board[Board.NextPlayer()].Board & ~Board.BoardOverview);

	int64 rook_moves = right_moves | left_moves | up_moves | down_moves;

	// To save space i removed the rook_captures and non captures variables.
	//
	//int64 rook_captures = rook_moves & Board.Board[Board.NextPlayer()].Board;
	//int64 rook_non_captures = rook_moves & ~Board.BoardOverview;
	down_moves = rook_moves & Board.Board[Board.NextPlayer()].Board;
	up_moves = rook_moves & ~Board.BoardOverview;

	rook_moves = down_moves | up_moves;

	return rook_moves;
}



int64 validator::genBishopMoves(board &Board, unsigned int &SourceSquare)
{
	int64 deg45Moves = Bishop[DEG45][SourceSquare] & Board.BoardOverview;
	deg45Moves = (deg45Moves<<9) | (deg45Moves<<18) | (deg45Moves<<27) | (deg45Moves<<36) | (deg45Moves<<45) | (deg45Moves<<54);
	deg45Moves = deg45Moves & Bishop[DEG45][SourceSquare];
	deg45Moves = (deg45Moves ^ Bishop[DEG45][SourceSquare]) ^ (Board.Board[Board.NextPlayer()].Board & ~Board.BoardOverview);

	int64 deg225Moves = Bishop[DEG225][SourceSquare] & Board.BoardOverview;
	deg225Moves = (deg225Moves>>9) | (deg225Moves>>18) | (deg225Moves>>27) | (deg225Moves>>36) | (deg225Moves>>45) | (deg225Moves>>54);
	deg225Moves = deg225Moves & Bishop[DEG225][SourceSquare];
	deg225Moves = (deg225Moves ^ Bishop[DEG225][SourceSquare]) ^ (Board.Board[Board.NextPlayer()].Board & ~Board.BoardOverview);

	int64 deg135Moves = Bishop[DEG135][SourceSquare] & Board.BoardOverview;
	deg135Moves = (deg135Moves<<7) | (deg135Moves<<14) | (deg135Moves<<21) | (deg135Moves<<28) | (deg135Moves<<35) | (deg135Moves<<42);
	deg135Moves = deg135Moves & Bishop[DEG135][SourceSquare];
	deg135Moves = (deg135Moves ^ Bishop[DEG135][SourceSquare]) ^ (Board.Board[Board.NextPlayer()].Board & ~Board.BoardOverview);

	int64 deg315Moves = Bishop[DEG315][SourceSquare] & Board.BoardOverview;
	deg315Moves = (deg315Moves>>7) | (deg315Moves>>14) | (deg315Moves>>21) | (deg315Moves>>28) | (deg315Moves>>35) | (deg315Moves>>42);
	deg315Moves = deg315Moves & Bishop[DEG315][SourceSquare];
	deg315Moves = (deg315Moves ^ Bishop[DEG315][SourceSquare]) ^ (Board.Board[Board.NextPlayer()].Board & ~Board.BoardOverview);

	int64 bishop_moves = deg45Moves | deg225Moves | deg135Moves | deg315Moves;
	//int64 bishop_captures = bishop_moves & Board.Board[Board.NextPlayer()].Board;
	//int64 bishop_non_captures = bishop_moves & ~Board.BoardOverview;

	deg315Moves = bishop_moves & Board.Board[Board.NextPlayer()].Board;
	deg135Moves = bishop_moves & ~Board.BoardOverview;

	bishop_moves = deg135Moves | deg315Moves;

	return bishop_moves;
}


int64 validator::genPawnMoves(board &Board, unsigned int &SourceSquare)
{

	//
	// We've already generated pawn capture moves
	//
	int64 pawnMoves = Moves[PAWN - Board.CurrentPlayer][SourceSquare] & Board.Board[Board.NextPlayer()].Board;


	//
	// Here we check the standard pawn moves.
	//	First we check if the square directly infront of the pawn
	//	is free, then we check if the pawn is on its starting rank,
	//	as it could move two spaces ahead.
	//
	unsigned int possibleMove = SourceSquare;
	possibleMove += (Board.CurrentPlayer == WHITE)? 8 : -8;

	//
	// Check one space ahead
	//
	if((Board.BoardOverview & side::SquareBits[possibleMove]) == 0)
	{
		pawnMoves |= side::SquareBits[possibleMove];

		//
		// Check if we are in the front rank
		//
		if(((SourceSquare / 8 == 1) && (Board.CurrentPlayer == WHITE)) || 
			((SourceSquare / 8 == 6) && (Board.CurrentPlayer == BLACK)))
		{
			possibleMove += (Board.CurrentPlayer == WHITE)? 8 : -8;
			//
			// Check two squares ahead is free
			//
			if((Board.BoardOverview & side::SquareBits[possibleMove]) == 0)
				pawnMoves |= side::SquareBits[possibleMove];
		}
	}

	return pawnMoves;
}




