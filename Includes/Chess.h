#ifndef __CHESS_H__
#define __CHESS_H__

#include "Common.h"
#include "Player.h"
#include "Validator.h"
#include <vector>
#include <string>


using std::vector;
using std::string;


class chess
{
	public:
		chess();
		void newGame(player &Player1, player &Player2);
		bool undoMove(string &Message);
		bool makeMove(string &Message);
		bool isGameOver();
		//char* getBoard();

		int Error;
		string ErrorMsg;

	private:
		void SetError(int error, string errorMsg);

		board *Board;
		player *Player[2];
		validator MoveValidator;
		bool undoAvaliable;
		bool gameReady;
		bool gameOver;

		vector<move> movesList;
};

#endif
