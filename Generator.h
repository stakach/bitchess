#ifndef __GENERATOR_H__
#define __GENERATOR_H__


#include "Common.h"
#include "Validator.h"
#include "Board.h"
#include <list>


using std::list;


class generator
{
	public:
		generator();
		list<move> generateMoves(board &Board, unsigned int sourceSquare);

	private:
		list<move> CurrentMoves;
};



#endif

