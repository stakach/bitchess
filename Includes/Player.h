#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Common.h"
#include "Board.h"



class player
{
	public:
		void setSide(unsigned int side){Side = side;}
		unsigned int getSide() {return Side;}

		virtual move getMove(board currentBoard) = 0; // Players: Human, AI, maybe interface with an online thing?

	protected:
		unsigned int Side;
};


#include <iostream>
using std::cin;
using std::string;
using std::cout;
using std::endl;

class human: public player
{
	public:
		human()
		{
		}
		move getMove(board currentBoard)
		{
			return Move;
		}
		void setMove(char *board)
		{
			cout << "\nSelect Piece: ";
			cin >> Move.SourceSquare;
			cout << "Select Destination: ";
			cin >> Move.DestinationSquare;
			Move.MoveType = MOVE_NORMAL;
			if(board[Move.SourceSquare] != ' ')
			{
				switch(board[Move.SourceSquare])
				{
					case 'P':
						Move.MovingPiece = PAWN;
						Move.RestingPiece = PAWN;
						if(board[Move.DestinationSquare] != ' ')
							Move.MoveType = MOVE_CAPTURE_ORDINARY;
						break;
					case 'R':
						Move.MovingPiece = ROOK;
						Move.RestingPiece = ROOK;
						break;
					case 'K':
						Move.MovingPiece = KING;
						Move.RestingPiece = KING;
						break;
					case 'Q':
						Move.MovingPiece = QUEEN;
						Move.RestingPiece = QUEEN;
						break;
					case 'B':
						Move.MovingPiece = BISHOP;
						Move.RestingPiece = BISHOP;
						break;
					case 'H':
						Move.MovingPiece = KNIGHT;
						Move.RestingPiece = KNIGHT;
				}
			}

		}

		move Move;
};

#endif
