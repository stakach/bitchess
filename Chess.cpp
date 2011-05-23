#include "stdafx.h"

#include "Includes\Chess.h"
#include "Includes\Board.h"
#include "Includes\Common.h"

#include <string>


using std::string;


chess::chess()
{
	//
	// Null the pointers and use set error to setup the game state.
	//
	Board = 0;
	Player[0] = 0;
	Player[1] = 0;

	SetError(-1, "Game not started");
}


void chess::newGame(player &Player1, player &Player2)
{
	//
	// If there was a game running, delete the board and clear the moves list.
	//
	if (Board != 0)
	{
		delete Board;
		Board = 0;
	}
	movesList.clear();

	//
	// Try and create a new board ready to go.
	//
	Board = new board();
	if (Board == 0)
	{
		SetError(-2, "Unable to start a new game. Out of Memory.");
		return;
	}

	//
	// Set the player pointers.
	//	(its up to the interface to control player memory if dynamically created)
	//
	Player[0] = &Player1;
	Player[0]->setSide(WHITE);

	Player[1] = &Player2;
	Player[1]->setSide(BLACK);

	//
	// Put the game in running state.
	//
	SetError(0, "Game functioning normally");
	gameReady = true;
	gameOver = false;
}


bool chess::makeMove(string &Message)
{
	bool noError = false;

	//
	// Check we can actually make a move, then ask the players
	//	for a move, validate it and apply the move.
	//
	if (gameReady && !gameOver)
	{
		move Move;

		//
		// Get current players move and validate it.
		//
		Move = Player[Board->CurrentPlayer]->getMove(*Board);
		move &LastMove = (movesList.empty())? Move : movesList.back();
		noError = MoveValidator.CheckMove(*Board, Move, Message, LastMove);

		//
		// If move validated try and apply the move.
		//
		if (noError)
		{
			noError = Board->ApplyMove(Move);
			movesList.push_back(Move);			// Add to moves list
			undoAvaliable = !movesList.empty();	// Set undo flag

			//
			// Check for a critical error, if none found get the game status
			//	eg. White is in check, Checkmate, Stalemate ect.
			//
			if (noError)
				gameOver = MoveValidator.CheckStatus(*Board, Message, Move);
			else
				SetError(1, "An unknown error occured whilst performing a move.");
		}
	}
	else
		Message = "Game not ready or game over";

	return noError;
}


bool chess::undoMove(string &Message)
{
	bool noError = false;

	//
	// Undoes last move made, sets undo avaliable flag and gets game status.
	//
	if (gameReady && undoAvaliable)
	{
		noError = Board->UndoMove(movesList.back());
		movesList.pop_back();
		undoAvaliable = !movesList.empty();

		move LastMove;
		if(undoAvaliable)
			LastMove = movesList.back();
		else
		{
			LastMove.CapturedPiece = NOTHING;
			LastMove.MovingPiece = NOTHING;
			LastMove.RestingPiece = NOTHING;
			LastMove.SourceSquare = 64;
			LastMove.DestinationSquare = 64;
			LastMove.MoveType = 100;
		}

		if (noError)
			gameOver = MoveValidator.CheckStatus(*Board, Message, LastMove);
		else
			SetError(2, "An unknown error occured whilst undoing a move.");
	}

	return noError;
}


inline void chess::SetError(int error, string errorMsg)
{
	//
	// Puts the game in an error state.
	//
	Error = error;
	ErrorMsg = errorMsg;
	gameReady = false;
	undoAvaliable = false;
	gameOver = true;
}


bool chess::isGameOver()
{
	return gameOver;
}


