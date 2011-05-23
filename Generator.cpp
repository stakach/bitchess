#include "stdafx.h"

#include "Includes\Validator.h"
#include "Includes\common.h"


generator::generator()
{
	//
	// Check if the moves lookup table has been generated.
	//
	if(!validator::generationComplete)
		validator::generateMoveBitmaps();
}


list<move> generator::generateMoves(board &Board, unsigned int sourceSquare)
{
      // Check for white moves, one piece type at a time
      // if any one type can capture the king, stop the work immediately
      // because the board position is illegal
      ComputeQueenMoves(Board, sourceSquare);
      ComputeKingMoves(Board, sourceSquare);
      ComputeRookMoves(Board, sourceSquare);
      if ( !ComputeWhiteBishopMoves( theBoard, jcBoard.WHITE_BISHOP ) ) return false;
      if ( !ComputeWhiteKnightMoves( theBoard ) ) return false;
      if ( !ComputeWhitePawnMoves( theBoard ) ) return false;
    }
    else  // Compute Black's moves
    {
      if ( theBoard.GetExtraKings( jcPlayer.SIDE_BLACK ) != 0 )
      {
        theBoard.ClearExtraKings( jcPlayer.SIDE_BLACK );
      }
      if ( !ComputeBlackQueenMoves( theBoard ) ) return false;
      if ( !ComputeBlackKingMoves( theBoard ) ) return false;
      if ( !ComputeBlackRookMoves( theBoard, jcBoard.BLACK_ROOK ) ) return false;
      if ( !ComputeBlackBishopMoves( theBoard, jcBoard.BLACK_BISHOP ) ) return false;
      if ( !ComputeBlackKnightMoves( theBoard ) ) return false;
      if ( !ComputeBlackPawnMoves( theBoard ) ) return false;
    }

    // And finally, if there are no pseudo-legal moves at all, we have an
    // obvious error (there are no pieces on the board!); flag the condition
    if ( Moves.size() == 0 )
      return false;
    return true;
    }

	return Moves;
}

