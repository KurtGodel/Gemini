//
//  MovementCalculator.cpp
//  Schach
//
//  Created by Morgan Redding on 8/10/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "MovementCalculator.h"

uint8_t MovementCalculator::calculateMoves(Move *_moves, bool userToMove)
{
    numberOfMoves = 0;
    moves = _moves;
    
    int8_t piece;
    Tile tile;
    
    for(uint8_t i = 0; i < 64; i++)
    {
        if(board.b[i] == 0)
        {
            continue;
        }
        if((board.b[i] < 0) != userToMove)
        {
            continue;
        }
        piece = abs(board.b[i]);
        tile = Tile(i);
        if(piece == 1)
        {
            pawn(tile, userToMove);
        }
        else if(piece == 2)
        {
            knight(tile, userToMove);
        }
        else if(piece == 3)
        {
            bishop(tile, userToMove);
        }
        else if(piece == 4)
        {
            rook(tile, userToMove);
        }
        else if(piece == 5)
        {
            queen(tile, userToMove);
        }
        else if(piece == 6)
        {
            king(tile, userToMove);
        }
    }
    return numberOfMoves;
}

void MovementCalculator::pawn(const Tile &tile, bool userToMove)
{
    if(userToMove)
    {
        if(board.b[tile.index + 1] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = tile.index + 1;
            numberOfMoves++;
            if(tile.rank == 1 && board.b[tile.index + 2] == 0)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = tile.index + 2;
                numberOfMoves++;
            }
        }
        if(tile.file != 0 && board.b[tile.index - 7] > 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = tile.index - 7;
            numberOfMoves++;
        }
        if(tile.file != 7 && board.b[tile.index + 9] > 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = tile.index + 9;
            numberOfMoves++;
        }
    }
    else
    {
        if(board.b[tile.index - 1] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = tile.index - 1;
            numberOfMoves++;
            if(tile.rank == 6 && board.b[tile.index - 2] == 0)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = tile.index - 2;
                numberOfMoves++;
            }
        }
        if(tile.file != 0 && board.b[tile.index - 9] < 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = tile.index - 9;
            numberOfMoves++;
        }
        if(tile.file != 7 && board.b[tile.index + 7] < 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = tile.index + 7;
            numberOfMoves++;
        }
    }
}

void MovementCalculator::knight(const Tile &tile, bool userToMove)
{
    uint64_t to64 = knightMovement[tile.index];
    if(userToMove)
    {
        to64 &= ~board.user;
    }
    else
    {
        to64 &= ~board.cpu;
    }
    uint8_t to;
    while(to64 != 0)
    {
        to = board.lsm(to64);
        to64 -= to64 & -to64;
        moves[numberOfMoves].from = tile.index;
        moves[numberOfMoves].to = to;
        numberOfMoves++;
    }
}

void MovementCalculator::bishop(const Tile& tile, bool userToMove)
{
    int8_t diagonalLength, index;
    
    diagonalLength = bishopNorthEast[tile.index];
    index = tile.index;
    for(uint8_t i = 1; i < diagonalLength; i++)
    {
        index -= 7;
        if(board.b[index] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = index;
            numberOfMoves++;
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = index;
                numberOfMoves++;
            }
            break;
        }
    }
    
    diagonalLength = bishopNorthWest[tile.index];
    index = tile.index;
    for(uint8_t i = 1; i < diagonalLength; i++)
    {
        index -= 9;
        if(board.b[index] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = index;
            numberOfMoves++;
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = index;
                numberOfMoves++;
            }
            break;
        }
    }
    
    diagonalLength = bishopSouthEast[tile.index];
    index = tile.index;
    for(uint8_t i = 1; i < diagonalLength; i++)
    {
        index += 9;
        if(board.b[index] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = index;
            numberOfMoves++;
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = index;
                numberOfMoves++;
            }
            break;
        }
    }
    
    diagonalLength = bishopSouthWest[tile.index];
    index = tile.index;
    for(uint8_t i = 1; i < diagonalLength; i++)
    {
        index += 7;
        if(board.b[index] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = index;
            numberOfMoves++;
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = index;
                numberOfMoves++;
            }
            break;
        }
    }
}

void MovementCalculator::rook(const Tile& tile, bool userToMove)
{
    int index;
    
    index = tile.index;
    for(int8_t i = tile.rank + 1; i < 8; i++) {
        index += 1;
        if(board.b[index] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = index;
            numberOfMoves++;
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = index;
                numberOfMoves++;
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.rank - 1; i >= 0; i--) {
        index -= 1;
        if(board.b[index] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = index;
            numberOfMoves++;
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = index;
                numberOfMoves++;
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.file + 1; i < 8; i++) {
        index += 8;
        if(board.b[index] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = index;
            numberOfMoves++;
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = index;
                numberOfMoves++;
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.file - 1; i >= 0; i--) {
        index -= 8;
        if(board.b[index] == 0)
        {
            moves[numberOfMoves].from = tile.index;
            moves[numberOfMoves].to = index;
            numberOfMoves++;
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = index;
                numberOfMoves++;
            }
            break;
        }
    }
}

void MovementCalculator::queen(const Tile& tile, bool userToMove)
{
    bishop(tile, userToMove);
    rook(tile, userToMove);
}

void MovementCalculator::king(const Tile& tile, bool userToMove)
{
    if(userToMove)
    {
        if(tile.index == 32)
        {
            if(board.castling[2] && board.b[8] == 0 && board.b[16] == 0 && board.b[24] == 0)
            {
                // queen-side
                moves[numberOfMoves].from = 32;
                moves[numberOfMoves].to = 16;
                numberOfMoves++;
            }
            if(board.castling[3] && board.b[48] == 0 && board.b[40] == 0)
            {
                // king-side
                moves[numberOfMoves].from = 32;
                moves[numberOfMoves].to = 48;
                numberOfMoves++;
            }
        }
    }
    else
    {
        if(tile.index == 39)
        {
            if(board.castling[0] && board.b[15] == 0 && board.b[23] == 0 && board.b[31] == 0)
            {
                // queen-side
                moves[numberOfMoves].from = 39;
                moves[numberOfMoves].to = 23;
                numberOfMoves++;
            }
            if(board.castling[1] && board.b[55] == 0 && board.b[47] == 0)
            {
                // king-side
                moves[numberOfMoves].from = 39;
                moves[numberOfMoves].to = 55;
                numberOfMoves++;
            }
        }
    }
    uint64_t to64 = kingMovement[tile.index];
    if(userToMove)
    {
        to64 &= ~board.user;
    }
    else
    {
        to64 &= ~board.cpu;
    }
    uint8_t to;
    while(to64 != 0)
    {
        to = board.lsm(to64);
        to64 -= to64 & -to64;
        moves[numberOfMoves].from = tile.index;
        moves[numberOfMoves].to = to;
        numberOfMoves++;
    }
}