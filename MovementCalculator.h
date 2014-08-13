//
//  MovementCalculator.h
//  Schach
//
//  Created by Morgan Redding on 8/10/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__MovementCalculator__
#define __Schach__MovementCalculator__

#include <iostream>

#include "Board.h"
#include "Move.h"
#include "Tile.h"

// TODO: prevent castling through check
// TODO: ignore moves that put you into check
// TODO: ignore moves that do not take you out of check (this might be the movement tree's responsibility)

class MovementCalculator
{
public:
    MovementCalculator(const Board& board) : board(board) {};
    uint8_t calculateMoves(Move* moves, bool userToMove);
private:
    const Board& board;
    Move* moves;
    bool userToMove;
    uint8_t numberOfMoves;
    
    void pawn(const Tile& tile, bool userToMove);
    void knight(const Tile& tile, bool userToMove);
    void bishop(const Tile& tile, bool userToMove);
    void rook(const Tile& tile, bool userToMove);
    void queen(const Tile& tile, bool userToMove);
    void king(const Tile& tile, bool userToMove);
    
    const uint64_t knightMovement[64] =
    {
        0x20400,
        0x50800,
        0xa1100,
        0x142200,
        0x284400,
        0x508800,
        0xa01000,
        0x402000,
        0x2040004,
        0x5080008,
        0xa110011,
        0x14220022,
        0x28440044,
        0x50880088,
        0xa0100010,
        0x40200020,
        0x204000402,
        0x508000805,
        0xa1100110a,
        0x1422002214,
        0x2844004428,
        0x5088008850,
        0xa0100010a0,
        0x4020002040,
        0x20400040200,
        0x50800080500,
        0xa1100110a00,
        0x142200221400,
        0x284400442800,
        0x508800885000,
        0xa0100010a000,
        0x402000204000,
        0x2040004020000,
        0x5080008050000,
        0xa1100110a0000,
        0x14220022140000,
        0x28440044280000,
        0x50880088500000,
        0xa0100010a00000,
        0x40200020400000,
        0x204000402000000,
        0x508000805000000,
        0xa1100110a000000,
        0x1422002214000000,
        0x2844004428000000,
        0x5088008850000000,
        0xa0100010a0000000,
        0x4020002040000000,
        0x400040200000000,
        0x800080500000000,
        0x1100110a00000000,
        0x2200221400000000,
        0x4400442800000000,
        0x8800885000000000,
        0x100010a000000000,
        0x2000204000000000,
        0x4020000000000,
        0x8050000000000,
        0x110a0000000000,
        0x22140000000000,
        0x44280000000000,
        0x88500000000000,
        0x10a00000000000,
        0x20400000000000
    };
    
    const uint64_t kingMovement[64] = {
        0x302,
        0x705,
        0xe0a,
        0x1c14,
        0x3828,
        0x7050,
        0xe0a0,
        0xc040,
        0x30203,
        0x70507,
        0xe0a0e,
        0x1c141c,
        0x382838,
        0x705070,
        0xe0a0e0,
        0xc040c0,
        0x3020300,
        0x7050700,
        0xe0a0e00,
        0x1c141c00,
        0x38283800,
        0x70507000,
        0xe0a0e000,
        0xc040c000,
        0x302030000,
        0x705070000,
        0xe0a0e0000,
        0x1c141c0000,
        0x3828380000,
        0x7050700000,
        0xe0a0e00000,
        0xc040c00000,
        0x30203000000,
        0x70507000000,
        0xe0a0e000000,
        0x1c141c000000,
        0x382838000000,
        0x705070000000,
        0xe0a0e0000000,
        0xc040c0000000,
        0x3020300000000,
        0x7050700000000,
        0xe0a0e00000000,
        0x1c141c00000000,
        0x38283800000000,
        0x70507000000000,
        0xe0a0e000000000,
        0xc040c000000000,
        0x302030000000000,
        0x705070000000000,
        0xe0a0e0000000000,
        0x1c141c0000000000,
        0x3828380000000000,
        0x7050700000000000,
        0xe0a0e00000000000,
        0xc040c00000000000,
        0x203000000000000,
        0x507000000000000,
        0xa0e000000000000,
        0x141c000000000000,
        0x2838000000000000,
        0x5070000000000000,
        0xa0e0000000000000,
        0x40c0000000000000,
    };
    
    const unsigned char lsmArray[64] = {
        0,  1,  2, 53,  3,  7, 54, 27,
        4, 38, 41,  8, 34, 55, 48, 28,
        62,  5, 39, 46, 44, 42, 22,  9,
        24, 35, 59, 56, 49, 18, 29, 11,
        63, 52,  6, 26, 37, 40, 33, 47,
        61, 45, 43, 21, 23, 58, 17, 10,
        51, 25, 36, 32, 60, 20, 57, 16,
        50, 31, 19, 15, 30, 14, 13, 12
    };
    
    const unsigned char bishopNorthWest[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 2, 2, 2, 2, 2, 2,
        0, 1, 2, 3, 3, 3, 3, 3,
        0, 1, 2, 3, 4, 4, 4, 4,
        0, 1, 2, 3, 4, 5, 5, 5,
        0, 1, 2, 3, 4, 5, 6, 6,
        0, 1, 2, 3, 4, 5, 6, 7
    };
    
    const unsigned char bishopNorthEast[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 0,
        2, 2, 2, 2, 2, 2, 1, 0,
        3, 3, 3, 3, 3, 2, 1, 0,
        4, 4, 4, 4, 3, 2, 1, 0,
        5, 5, 5, 4, 3, 2, 1, 0,
        6, 6, 5, 4, 3, 2, 1, 0,
        7, 6, 5, 4, 3, 2, 1, 0
    };
    
    const unsigned char bishopSouthWest[64] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        0, 1, 2, 3, 4, 5, 6, 6,
        0, 1, 2, 3, 4, 5, 5, 5,
        0, 1, 2, 3, 4, 4, 4, 4,
        0, 1, 2, 3, 3, 3, 3, 3,
        0, 1, 2, 2, 2, 2, 2, 2,
        0, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    
    const unsigned char bishopSouthEast[64] = {
        7, 6, 5, 4, 3, 2, 1, 0,
        6, 6, 5, 4, 3, 2, 1, 0,
        5, 5, 5, 4, 3, 2, 1, 0,
        4, 4, 4, 4, 3, 2, 1, 0,
        3, 3, 3, 3, 3, 2, 1, 0,
        2, 2, 2, 2, 2, 2, 1, 0,
        1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };
};

#endif /* defined(__Schach__MovementCalculator__) */
