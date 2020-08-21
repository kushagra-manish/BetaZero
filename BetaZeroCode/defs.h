#ifndef DEFS_H // if DEFS_H is not defined we're defining DEFS_H as
#define DEFS_H // the whole code underneath is the definition

#include "stdlib.h"

// #define DEBUG
#ifndef DEBUG
	#define ASSERT(n)
#else
    // for debugging purposes
	#define ASSERT(n) \
	if(!(n)) { \
		printf("%s - Failed",#n); \
		printf("On %s ",__DATE__); \
		printf("At %s ",__TIME__); \
		printf("In File %s ",__FILE__); \
		printf("At Line %d\n",__LINE__); \
		exit(1);}
#endif

typedef unsigned long long U64;

#define NAME "BetaZero 1.0"
#define BRD_SQ_NUM 120 //120 size array
#define MAXGAMEMOVES 2048 // MAximum moves in  a game , worst case, normally games are way less
#define MAXPOSITIONMOVES 256
#define MAXDEPTH 64
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" //starting FEN string

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
    A8= 91 , B8 , C8 , D8 , E8 , F8 , G8 , H8 , NO_SQ , OFFBOARD// no square and offboard
};

// easy 0 for FALSE , 1 for TRUE , easier to compare when using ==
enum { FALSE , TRUE };

// variables for White Kingside Castle , White Queenside castle and same for black (four bits) 
enum { WKCA = 1 , WQCA = 2, BKCA=4 , BQCA=8};

typedef struct{
    int move;
    int score; // for move ordering
}S_MOVE;

typedef struct {
	S_MOVE moves[MAXPOSITIONMOVES];
	int count;
} S_MOVELIST;

typedef struct{
    U64 posKey;
    int move;
} S_PVENTRY;

typedef struct{
    S_PVENTRY *pTable;
    int numEntries;
} S_PVTABLE;


// struct that contains the info to undo a move
typedef struct {
    U64 posKey; // the position key
    int move; // move that was just played
    int castlePerm; // castling permission
    int enPas; // enpassant square, if it was set
    int fiftyMove; //status of the fiftyMove rule

}S_UNDO ;

// struct that contains the board
typedef struct
{
    // the board
    int pieces[BRD_SQ_NUM];

    // all stuff from here used for position evaluation

    U64 posKey; // unique key generated for each position 
    U64 pawns[3]; //indicates whether pawns are present for white,black,both in binary (0 doesn't exist, 1 exists) 
    
    int KingSq[2]; // indicates the position of the kings
    
    int side; // shows which side to move
    int enPas; // shows whether the square is ready for en passant
    int fiftyMove; // checks whether fifty move rule is available for a draw
    int castlePerm; // shows what castling moves are allowed for both sides (binary) 
    
    int ply; // ply => half moves , shows uptil which ply is the position being evaluated
    int hisPly; // keeps a history of plys for threefold repetition draws
    
    int pceNum[13]; // number of pieces that we have on the board
    int bigPce[2]; // number of Big pieces that we have (everything except pawns)
    int majPce[2]; // number of major pieces that we have (rooks and queens)
    int minPce[2]; // number of minor pieces that we have (bishops and knights)
    int material[2]; // material scores for black and white

    S_UNDO history[MAXGAMEMOVES]; //stores the history
    
    int pList[13][10]; // piece list for 13 possible pieces plus worst case 10 for each piece. like if all 8 pawns promote , you could have 10 rooks/knights/bishops
    S_PVTABLE PvTable[1];
    int PvArray[MAXDEPTH];

    int searchHistory[13][BRD_SQ_NUM];
    int searchKillers[2][MAXDEPTH];
}S_BOARD;

typedef struct{
    int starttime;
    int stoptime;
    int depth;
    int depthset;
    int timeset;
    int movestogo;
    int infinite;

    long nodes;
    
    int quit;
    int stopped;

    
	float fh;
	float fhf;

} S_SEARCHINFO;

/* GAME MOVE */


    /*   
    0000 0000 0000 0000 0000 0111 1111 -> From 0x7F
    0000 0000 0000 0011 1111 1000 0000 -> To >> 7, 0x7F
    0000 0000 0011 1100 0000 0000 0000 -> Captured >> 14, 0xF
    0000 0000 0100 0000 0000 0000 0000 -> EP 0x40000
    0000 0000 1000 0000 0000 0000 0000 -> Pawn Start 0x80000
    0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece >> 20, 0xF
    0001 0000 0000 0000 0000 0000 0000 -> Castle 0x1000000
    */


    #define FROMSQ(m) ((m) & 0x7F)
    #define TOSQ(m) (((m)>>7) & 0x7F)
    #define CAPTURED(m) (((m)>>14) & 0xF)
    #define PROMOTED(m) (((m)>>20) & 0xF)

    #define MFLAGEP 0x40000
    #define MFLAGPS 0x80000
    #define MFLAGCA 0x1000000

    #define MFLAGCAP 0x7C000
    #define MFLAGPROM 0xF00000

    #define NOMOVE 0

/* MACROS */

    // returns corresponding square for file number, rank number
    #define FR2SQ(f,r) ( (21+(f) ) +( (r)*10 ) ) 
    // coverts 120 based sq to 64
    #define SQ64(sq120) (Sq120ToSq64[(sq120)])
    // the inverse
    #define SQ120(sq64) (Sq64ToSq120[(sq64)])
    // pop's the least significant bit
    #define POP(b) PopBit(b)
    // counts the set bits
    #define CNT(b) CountBits(b)

    //clears the bit on square sq
    #define CLRBIT(bb,sq) ((bb) &= ClearMask[(sq)])
    // sets the bit on sqaure sq
    #define SETBIT(bb,sq) ((bb) |= SetMask[(sq)])

    //Function for easy access  
    #define IsBQ(p) (PieceBishopQueen[(p)])
    #define IsRQ(p) (PieceRookQueen[(p)])
    #define IsKn(p) (PieceKnight[(p)])
    #define IsKi(p) (PieceKing[(p)])



/* GLOBALS */

    extern int Sq120ToSq64[BRD_SQ_NUM]; // array that converts the 120 based index of array into 64
    extern int Sq64ToSq120[64]; // array that converts 64 based index to 120 based
    extern U64 SetMask[64]; //set bit to 1
    extern U64 ClearMask[64]; // set bit to 0

    extern U64 PieceKeys[13][120]; // hashes for every piece on every index 
    extern U64 SideKey; // hash for which side to move
    extern U64 CastleKeys[16]; // hash for castling

    extern char PceChar[]; //piece char
    extern char SideChar[]; // side character
    extern char RankChar[]; // rank character
    extern char FileChar[]; // file character

    extern int PieceBig[13]; // Tells if a piece is big.. rest similar
    extern int PieceMaj[13];
    extern int PieceMin[13];
    extern int PieceVal[13];
    extern int PieceCol[13];

    // convert square number to file,rank
    extern int FilesBrd[BRD_SQ_NUM];
    extern int RanksBrd[BRD_SQ_NUM];

    //is the piece a Knight,king,rook/queen,bishop/queen    
    extern int PieceKnight[13];
    extern int PieceKing[13];
    extern int PieceRookQueen[13];
    extern int PieceBishopQueen[13];
    extern int PieceSlides[13];
    extern int PiecePawn[13];

/* FUNCTIONS */

    //init.cpp
    extern void AllInit();

    //bitboards.cpp
    extern void PrintBitBoard(U64 bb); // print the bit board
    extern int PopBit(U64 *bb); // Pops the LSB
    extern int CountBits(U64 b); // counts all the set bits

    // hashkeys.cpp
    extern U64 GeneratePosKey(const S_BOARD *pos); //generates a hash for a position

    // board.cpp
    extern void ResetBoard(S_BOARD *pos); // resets board to being completely empty
    extern int ParseFen(char *fen, S_BOARD *pos); // parses a char * / string and puts the position on board pos
    extern void PrintBoard(const S_BOARD *pos); // prints the current position
    extern void UpdateListsMaterial(S_BOARD *pos);// updates material after move or change in position
    extern int CheckBoard(const S_BOARD *pos);// checks if everything on board is according to position

    // attack.cpp
    extern int SqAttacked(const int sq, const int side, const S_BOARD *pos); // tells if a square is attacked

    // io.cpp
    extern char *PrMove(const int move);
    extern char *PrSq(const int sq);
    extern void PrintMoveList(const S_MOVELIST *list);
    extern int ParseMove(char *ptrChar, S_BOARD *pos);


    //validate.cpp
    extern int SqOnBoard(const int sq);
    extern int SideValid(const int side);
    extern int FileRankValid(const int fr);
    extern int PieceValidEmpty(const int pce);
    extern int PieceValid(const int pce);

    
    // movegen.cpp
    extern void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list);
    extern void GenerateAllCaps(const S_BOARD *pos, S_MOVELIST *list);
    extern int MoveExists(S_BOARD *pos, const int move);
    extern int InitMvvLva();
    
    // makemove.cpp
    extern int MakeMove(S_BOARD *pos, int move);
    extern void TakeMove(S_BOARD *pos);

    
    // perft.cpp
    extern void PerftTest(int depth, S_BOARD *pos);

    // search.cpp
    extern void SearchPosition(S_BOARD *pos,  S_SEARCHINFO *info);

    // misc.cpp 
    extern int GetTimeMs();
    extern void ReadInput(S_SEARCHINFO *info);
    
    // pvtable.cpp
    extern void InitPvTable(S_PVTABLE *table);
    extern void StorePvMove(const S_BOARD *pos, const int move);
    extern int ProbePvTable(const S_BOARD *pos);
    extern int GetPvLine(const int depth, S_BOARD *pos);
    extern void ClearPvTable(S_PVTABLE *table);

    //evaluate.cpp
    extern int EvalPosition(const S_BOARD *pos);
    
    // uci.cpp 
    extern void Uci_Loop();


#endif
