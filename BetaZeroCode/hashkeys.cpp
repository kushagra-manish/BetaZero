// hashkeys.cpp
#include<bits/stdc++.h>
#include "defs.h"

U64 GeneratePosKey(const S_BOARD *pos) {
    // intialization
	int sq = 0;
	U64 finalKey = 0;
	int piece = EMPTY;
	
	// pieces
    // if for every valid square, there is a valid piece, xor it to the hash
	for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
		piece = pos->pieces[sq];
		if(piece!=NO_SQ && piece!=EMPTY && piece != OFFBOARD) {
			ASSERT(piece>=wP && piece<=bK);
			finalKey ^= PieceKeys[piece][sq];
		}		
	}
	
    // if white to play, hash the white key
	if(pos->side == WHITE) {
		finalKey ^= SideKey;
	}
	
    //if enpassant is active, hash the enpassant sqaure key
	if(pos->enPas != NO_SQ) {
		ASSERT(pos->enPas>=0 && pos->enPas<BRD_SQ_NUM);
		finalKey ^= PieceKeys[EMPTY][pos->enPas];
	}
	
	ASSERT(pos->castlePerm>=0 && pos->castlePerm<=15);
	
    // hash the castling permissions
	finalKey ^= CastleKeys[pos->castlePerm];
	
	return finalKey;
}