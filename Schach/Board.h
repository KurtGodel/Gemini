//
//  Board.h
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__Board__
#define __Schach__Board__

#include <iostream>
#include <vector>
#include "Node.h"
#include "Move.h"
#include "PieceConstants.h"

class Board
{
public:
    Board();
    Board(const Board& board);
    
    char b[64];
    /*
     human                      cpu
     -4, -1, 0,  0,  0,  0,  1,  4,
     -2, -1, 0,  0,  0,  0,  1,  2,
     -3, -1, 0,  0,  0,  0,  1,  3,
     -5, -1, 0,  0,  0,  0,  1,  5,
     -6, -1, 0,  0,  0,  0,  1,  6,
     -3, -1, 0,  0,  0,  0,  1,  3,
     -2, -1, 0,  0,  0,  0,  1,  2,
     -4, -1, 0,  0,  0,  0,  1,  4,
     
     the board representation NEVER changes - when the human plays as black, the board remains the exact same
     */
    bool castling[4];           // 1,1,1,1      queen-side cpu, king-side cpu, queen-side human, king-side human
    int lastMove[3];            // 0,0          from, to
    int fiftyMoveDrawCounter;   // 0            counts ply; therefore, a draw may be declared when fiftyMoveDrawCounter>=100
    uint64_t getZorbistHash();
    bool set(char* arr);
    bool set(const Board &newBoard);
    std::string toString();
    void setRandomArr();
    
    const uint8_t USER_KING     = 0;
    const uint8_t USER_QUEEN    = 1;
    const uint8_t USER_ROOK     = 2;
    const uint8_t USER_BISHIP   = 3;
    const uint8_t USER_KNIGHT   = 4;
    const uint8_t USER_PAWN     = 5;
    const uint8_t EMPTY         = 6;
    const uint8_t CPU_PAWN      = 7;
    const uint8_t CPU_KNIGHT    = 8;
    const uint8_t CPU_BISHOP    = 9;
    const uint8_t CPU_ROOK      = 10;
    const uint8_t CPU_QUEEN     = 11;
    const uint8_t CPU_KING      = 12;
    uint64_t bitmaps[13];
    
    uint64_t cpu;
    uint64_t user;
    
    Board & operator=(Board b);
    bool operator==(const Board& board);
    bool operator!=(const Board& board);
    
    uint8_t lsm(const uint64_t& input) const;
    
    const uint8_t lsmArray[64] = {
        0,  1,  2, 53,  3,  7, 54, 27,
        4, 38, 41,  8, 34, 55, 48, 28,
        62,  5, 39, 46, 44, 42, 22,  9,
        24, 35, 59, 56, 49, 18, 29, 11,
        63, 52,  6, 26, 37, 40, 33, 47,
        61, 45, 43, 21, 23, 58, 17, 10,
        51, 25, 36, 32, 60, 20, 57, 16,
        50, 31, 19, 15, 30, 14, 13, 12
    };
    
    std::string printBitmap(uint64_t bitmap);
    
    void makeMove(const Move& move);
    void unmakeMove();
    void switchSides();
    std::vector<int32_t> history; // from, to, replaced, castling, fifty moves
private:
    void recalculateBitboards();
    std::vector<Board> debugHistory;
    void updateTile(uint8_t from, int8_t replace);
};

#endif /* defined(__Schach__Board__) */
