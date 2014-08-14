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
    
    Tile tile;
    uint64_t myPieces;
    
    int8_t enPassantFilePotential = -1;
    
    if(userToMove)
    {
        if(board.lastMove[0] - board.lastMove[1] == 2)
        {
            if(board.b[board.lastMove[1]] == -1)
            {
                enPassantFilePotential = board.lastMove[1];
            }
        }
        
        myPieces = board.bitmaps[Board::USER_PAWN];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            pawn(tile, userToMove, enPassantFilePotential);
        }
        
        myPieces = board.bitmaps[Board::USER_KNIGHT];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            knight(tile, userToMove);
        }
        
        myPieces = board.bitmaps[Board::USER_BISHIP];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            bishop(tile, userToMove);
        }
        
        myPieces = board.bitmaps[Board::USER_ROOK];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            rook(tile, userToMove);
        }
        
        myPieces = board.bitmaps[Board::USER_QUEEN];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            queen(tile, userToMove);
        }
        
        myPieces = board.bitmaps[Board::USER_KING];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            king(tile, userToMove);
        }
    }
    else
    {
        if(board.lastMove[1] - board.lastMove[0] == 2)
        {
            if(board.b[board.lastMove[1]] == 1)
            {
                enPassantFilePotential = board.lastMove[1];
            }
        }
        
        myPieces = board.bitmaps[Board::CPU_PAWN];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            pawn(tile, userToMove, enPassantFilePotential);
        }
        
        myPieces = board.bitmaps[Board::CPU_KNIGHT];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            knight(tile, userToMove);
        }
        
        myPieces = board.bitmaps[Board::CPU_BISHOP];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            bishop(tile, userToMove);
        }
        
        myPieces = board.bitmaps[Board::CPU_ROOK];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            rook(tile, userToMove);
        }
        
        myPieces = board.bitmaps[Board::CPU_QUEEN];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            queen(tile, userToMove);
        }
        
        myPieces = board.bitmaps[Board::CPU_KING];
        while(myPieces != 0)
        {
            tile = Tile(decrementBitboard(myPieces));
            king(tile, userToMove);
        }
    }
    return numberOfMoves;
}

void MovementCalculator::pawn(const Tile &tile, bool userToMove, int8_t enPassantIndex)
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
        if(enPassantIndex != -1)
        {
            if(abs(tile.index - enPassantIndex) < 10)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = enPassantIndex + 1;
                numberOfMoves++;
            }
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
        if(enPassantIndex != -1)
        {
            if(abs(tile.index - enPassantIndex) < 10)
            {
                moves[numberOfMoves].from = tile.index;
                moves[numberOfMoves].to = enPassantIndex - 1;
                numberOfMoves++;
            }
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
        to = decrementBitboard(to64);
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
    for(uint8_t i = 0; i < diagonalLength; i++)
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
    for(uint8_t i = 0; i < diagonalLength; i++)
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
                if(!isSquareInCheck(Tile(8), !userToMove) && !isSquareInCheck(Tile(16), !userToMove) && !!isSquareInCheck(Tile(24), !userToMove))
                {
                    moves[numberOfMoves].from = 32;
                    moves[numberOfMoves].to = 16;
                    numberOfMoves++;
                }
            }
            if(board.castling[3] && board.b[48] == 0 && board.b[40] == 0)
            {
                // king-side
                if(!isSquareInCheck(Tile(40), !userToMove) && !isSquareInCheck(Tile(48), !userToMove))
                {
                    moves[numberOfMoves].from = 32;
                    moves[numberOfMoves].to = 48;
                    numberOfMoves++;
                }
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
                if(!isSquareInCheck(Tile(15), !userToMove) && !isSquareInCheck(Tile(23), !userToMove) && !isSquareInCheck(Tile(31), !userToMove))
                {
                    moves[numberOfMoves].from = 39;
                    moves[numberOfMoves].to = 23;
                    numberOfMoves++;
                }
            }
            if(board.castling[1] && board.b[55] == 0 && board.b[47] == 0)
            {
                // king-side
                if(!isSquareInCheck(Tile(47), !userToMove) && !isSquareInCheck(Tile(55), !userToMove))
                {
                    moves[numberOfMoves].from = 39;
                    moves[numberOfMoves].to = 55;
                    numberOfMoves++;
                }
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
        to = decrementBitboard(to64);
        moves[numberOfMoves].from = tile.index;
        moves[numberOfMoves].to = to;
        numberOfMoves++;
    }
}

uint8_t MovementCalculator::lsm(const uint64_t& input) const
{
    return lsmArray[((input & -input) * 0x022fdd63cc95386d) >> 58];
}

uint8_t MovementCalculator::decrementBitboard(uint64_t& input) const
{
    uint64_t r = input & -input;
    input -= r;
    return lsmArray[(r * 0x022fdd63cc95386d) >> 58];
}

bool MovementCalculator::isSquareInCheck(Tile tile, bool userToMove)
{
    if(userToMove)
    {
        if((knightMovement[tile.index] & board.bitmaps[USER_KNIGHT + 6]) != 0)
        {
            return true;
        }
        if(tile.file != 0)
        {
            if(board.b[tile.index - 9] == -1)
            {
                return true;
            }
        }
        if(tile.file != 7)
        {
            if(board.b[tile.index + 7] == -1)
            {
                return true;
            }
        }
    }
    else
    {
        if((knightMovement[tile.index] & board.bitmaps[CPU_KNIGHT + 6]) != 0)
        {
            return true;
        }
        if(tile.file != 0)
        {
            if(board.b[tile.index - 7] == -1)
            {
                return true;
            }
        }
        if(tile.file != 7)
        {
            if(board.b[tile.index + 9] == -1)
            {
                return true;
            }
        }
    }
    
    int8_t diagonalLength, index, movingBishop, movingRook, movingQueen;
    if(userToMove)
    {
        movingBishop = USER_BISHOP;
        movingRook = USER_ROOK;
        movingQueen = USER_QUEEN;
    }
    else
    {
        movingBishop = CPU_BISHOP;
        movingRook = CPU_ROOK;
        movingQueen = CPU_QUEEN;
    }
    
    diagonalLength = bishopNorthEast[tile.index];
    index = tile.index;
    for(uint8_t i = 1; i < diagonalLength; i++)
    {
        index -= 7;
        if(board.b[index] != 0)
        {
            if(board.b[index] == movingBishop || board.b[index] == movingQueen)
            {
                return true;
            }
            break;
        }
    }
    
    diagonalLength = bishopNorthWest[tile.index];
    index = tile.index;
    for(uint8_t i = 0; i < diagonalLength; i++)
    {
        index -= 9;
        if(board.b[index] != 0)
        {
            if(board.b[index] == movingBishop || board.b[index] == movingQueen)
            {
                return true;
            }
            break;
        }
    }
    
    diagonalLength = bishopSouthEast[tile.index];
    index = tile.index;
    for(uint8_t i = 0; i < diagonalLength; i++)
    {
        index += 9;
        if(board.b[index] != 0)
        {
            if(board.b[index] == movingBishop || board.b[index] == movingQueen)
            {
                return true;
            }
            break;
        }
    }
    
    diagonalLength = bishopSouthWest[tile.index];
    index = tile.index;
    for(uint8_t i = 1; i < diagonalLength; i++)
    {
        index += 7;
        if(board.b[index] != 0)
        {
            if(board.b[index] == movingBishop || board.b[index] == movingQueen)
            {
                return true;
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.rank + 1; i < 8; i++) {
        index += 1;
        if(board.b[index] != 0)
        {
            if(board.b[index] == movingQueen || board.b[index] == movingRook)
            {
                return true;
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.rank - 1; i >= 0; i--) {
        index -= 1;
        if(board.b[index] != 0)
        {
            if(board.b[index] == movingQueen || board.b[index] == movingRook)
            {
                return true;
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.file + 1; i < 8; i++) {
        index += 8;
        if(board.b[index] != 0)
        {
            if(board.b[index] == movingQueen || board.b[index] == movingRook)
            {
                return true;
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.file - 1; i >= 0; i--) {
        index -= 8;
        if(board.b[index] != 0)
        {
            if(board.b[index] == movingQueen || board.b[index] == movingRook)
            {
                return true;
            }
            break;
        }
    }
    
    return false;
}