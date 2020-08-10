#ifndef DEFS_H // if DEFS_H is not defined we're defining DEFS_H as
#define DEFS_H // the whole code underneath is the definition

typedef unsigned long long U64;

#define NAME "BetaZero 1.0"
#define BRD_SQ_NUM 120 

// enumeration for easy access of pieces, wP= white pawn, wN = white knight and so on.
enum { EMPTY, wP , wN , wB , wR , wQ , wK , bP , bN , bB , bR , bQ , bK };
// enumeration for easy access of Files
enum { FILE_A , FILE_B , FILE_C, FILE_D , FILE_E , FILE_F , FILE_G , FILE_H , FILE_NONE };
// enumeration for easy access of Ranks
enum { RANK_1 , RANK_2 ,RANK_3 , RANK_4 , RANK_5 , RANK_6 , RANK_7 , RANK_8 , RANK_NONE };
// enumeration for easy access of piece color
enum { WHITE , BLACK , BOTH };

// enumeration for easy indexing of 120 element array we're taking as the board.
// 120 because the knights in the first rank or the last rank could jump into two ranks out of bounds.
// so wrapping the top and bottom layer twice , and the sides with one layer
// one wrap is necessary to tell all pieces that the square to move is illegal (out of the board)
enum {
    A1= 21 , B1 , C1 , D1 , E1 , F1 , G1 , H1 ,
    A2= 31 , B2 , C2 , D2 , E2 , F2 , G2 , H2 ,
    A3= 41 , B3 , C3 , D3 , E3 , F3 , G3 , H3 ,
    A4= 51 , B4 , C4 , D4 , E4 , F4 , G4 , H4 ,
    A5= 61 , B5 , C5 , D5 , E5 , F5 , G5 , H5 ,
    A6= 71 , B6 , C6 , D6 , E6 , F6 , G6 , H6 ,
    A7= 81 , B7 , C7 , D7 , E7 , F7 , G7 , H7 ,
    A8= 91 , B8 , C8 , D8 , E8 , F8 , G8 , H8 , NO_SQ // no square
};

// easy 0 for FALSE , 1 for TRUE , easier to compare when using ==
enum { FALSE , TRUE };

typedef struct
{
    // the board
    int pieces[BRD_SQ_NUM];

    // all stuff from here used for position evaluation

    U64 pawns[3]; //indicates whether pawns are present for white,black,both in binary (0 doesn't exist, 1 exists) 
    U64 posKey; // unique key generated for each position
    int KingSq[2]; // indicates the position of the kings
    int side; // shows which side to move
    int enPas; // shows whether the square is ready for en passant
    int fiftyMove; // checks whether fifty move rule is available for a draw
    int ply; // ply => half moves , shows uptil which ply is the position being evaluated
    int hisPly; // keeps a history of plys for threefold repetition draws
    int pceNum[13]; // number of pieces that we have on the board
    int bigPce[13]; // number of Big pieces that we have (everything except pawns)
    int majPce[13]; // number of major pieces that we have (rooks and queens)
    int minPce[13]; // number of minor pieces that we have (bishops and knights)


}S_BOARD;




#endif
