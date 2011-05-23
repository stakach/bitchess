// BitChess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::string;

#include "includes\chess.h"
#include "includes\Common.h"

int _tmain(int argc, _TCHAR* argv[])
{
	string moveMessage;
	human Player1;
	chess Chess;
	Chess.newGame(Player1, Player1);
	char pieces[64] = {
		'R','H','B','Q','K','B','H', 'R',
		'P','P','P','P','P','P','P', 'P',
		' ',' ',' ',' ',' ',' ',' ', ' ',
		' ',' ',' ',' ',' ',' ',' ', ' ',
		' ',' ',' ',' ',' ',' ',' ', ' ',
		' ',' ',' ',' ',' ',' ',' ', ' ',
		'P','P','P','P','P','P','P', 'P',
		'R','H','B','Q','K','B','H', 'R'
	};

	bool loop;
	while(!Chess.isGameOver())
	{
		for(int i = 0, j = 0, k = 0; i < 64; i++, j = i/8)
		{
			if(j > k)
			{
				cout << endl;
				k++;
			}
			
			//cout.fill('_');
			if(pieces[i] != ' ')
			{
				cout.width(4);
				cout << i;
				cout << pieces[i];
			}
			else
			{
				cout.width(5);
				cout << i;
			}
		}

		loop = true;

		while(loop)
		{
			Player1.setMove(pieces);
			moveMessage = "";
			if(Chess.makeMove(moveMessage))
			{
				loop = false;
				pieces[Player1.Move.DestinationSquare] = pieces[Player1.Move.SourceSquare];
				pieces[Player1.Move.SourceSquare] = ' ';
			}
			cout << moveMessage << endl;
		}

		cout << endl;

	}
}

