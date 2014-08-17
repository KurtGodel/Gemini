//
//  MovementCalculator.cpp
//  Schach
//
//  Created by Morgan Redding on 8/10/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "MovementCalculator.h"

void MovementCalculator::addMove(uint16_t from, uint16_t to)
{
    moves[numberOfMoves].from = from;
    moves[numberOfMoves].to = to;
    board.makeMove(moves[numberOfMoves]);
    
    Tile myKing;
    if(userToMove)
    {
        myKing = Tile(lsm(board.bitmaps[Board::USER_KING]));
    }
    else
    {
        myKing = Tile(lsm(board.bitmaps[Board::CPU_KING]));
    }
    
    if(isSquareInCheck(myKing, !userToMove))
    {
        return;
    }
    numberOfMoves++;
}

uint8_t MovementCalculator::calculateMovesInCheck(bool userToMove)
{
    // find what the check-threats are
        // you can never be in check from multiple knights
        // you can never be in check from multiple bishops
        // you can never be in check from multiple rooks
    
    // TODO
    
    return 0;
}

uint8_t MovementCalculator::calculateMoves(Move *_moves, bool userToMove)
{
    numberOfMoves = 0;
    moves = _moves;
    
    Tile tile;
    uint64_t myPieces;
    
    int8_t enPassantFilePotential = -1;
    
    if(userToMove)
    {
//        if(isSquareInCheck(Tile(lsm(board.bitmaps[Board::USER_KING])), !userToMove))
//        {
//            return calculateMovesInCheck(userToMove);
//        }
        
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
//        if(isSquareInCheck(Tile(lsm(board.bitmaps[Board::CPU_KING])), !userToMove))
//        {
//            return calculateMovesInCheck(userToMove);
//        }
        
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
        if(tile.rank == 6)
        {
            if(board.b[tile.index + 1] == 0)
            {
                addMove(tile.index + 200, tile.index + 1);
                addMove(tile.index + 300, tile.index + 1);
                addMove(tile.index + 400, tile.index + 1);
                addMove(tile.index + 500, tile.index + 1);
            }
            if(tile.file != 0 && board.b[tile.index - 7] > 0)
            {
                addMove(tile.index + 200, tile.index - 7);
                addMove(tile.index + 300, tile.index - 7);
                addMove(tile.index + 400, tile.index - 7);
                addMove(tile.index + 500, tile.index - 7);
            }
            if(tile.file != 7 && board.b[tile.index + 9] > 0)
            {
                addMove(tile.index + 200, tile.index + 9);
                addMove(tile.index + 300, tile.index + 9);
                addMove(tile.index + 400, tile.index + 9);
                addMove(tile.index + 500, tile.index + 9);
            }
        }
        else
        {
            if(board.b[tile.index + 1] == 0)
            {
                addMove(tile.index, tile.index + 1);
                if(tile.rank == 1 && board.b[tile.index + 2] == 0)
                {
                    addMove(tile.index, tile.index + 2);
                }
            }
            if(tile.file != 0 && board.b[tile.index - 7] > 0)
            {
                addMove(tile.index, tile.index - 7);
            }
            if(tile.file != 7 && board.b[tile.index + 9] > 0)
            {
                addMove(tile.index, tile.index + 9);
            }
            if(enPassantIndex != -1)
            {
                if(abs(tile.index - enPassantIndex) < 10)
                {
                    addMove(tile.index, enPassantIndex + 1);
                }
            }
        }
    }
    else
    {
        if(tile.rank == 1)
        {
            if(board.b[tile.index - 1] == 0)
            {
                addMove(tile.index + 200, tile.index - 1);
                addMove(tile.index + 300, tile.index - 1);
                addMove(tile.index + 400, tile.index - 1);
                addMove(tile.index + 500, tile.index - 1);
            }
            if(tile.file != 0 && board.b[tile.index - 9] < 0)
            {
                addMove(tile.index + 200, tile.index - 9);
                addMove(tile.index + 300, tile.index - 9);
                addMove(tile.index + 400, tile.index - 9);
                addMove(tile.index + 500, tile.index - 9);
            }
            if(tile.file != 7 && board.b[tile.index + 7] < 0)
            {
                addMove(tile.index + 200, tile.index + 7);
                addMove(tile.index + 300, tile.index + 7);
                addMove(tile.index + 400, tile.index + 7);
                addMove(tile.index + 500, tile.index + 7);
            }
        }
        else
        {
            if(board.b[tile.index - 1] == 0)
            {
                addMove(tile.index, tile.index - 1);
                if(tile.rank == 6 && board.b[tile.index - 2] == 0)
                {
                    addMove(tile.index, tile.index - 2);
                }
            }
            if(tile.file != 0 && board.b[tile.index - 9] < 0)
            {
                addMove(tile.index, tile.index - 9);
            }
            if(tile.file != 7 && board.b[tile.index + 7] < 0)
            {
                addMove(tile.index, tile.index + 7);
            }
            if(enPassantIndex != -1)
            {
                if(abs(tile.index - enPassantIndex) < 10)
                {
                    addMove(tile.index, enPassantIndex - 1);
                }
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
    while(to64 != 0)
    {
        addMove(tile.index, decrementBitboard(to64));
    }
}

void MovementCalculator::bishop(const Tile& tile, bool userToMove)
{
    int8_t diagonalLength, index;
    
    diagonalLength = bishopNorthEast[tile.index];
    index = tile.index;
    for(uint8_t i = 0; i < diagonalLength; i++)
    {
        index -= 7;
        if(board.b[index] == 0)
        {
            addMove(tile.index, index);
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                addMove(tile.index, index);
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
            addMove(tile.index, index);
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                addMove(tile.index, index);
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
            addMove(tile.index, index);
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                addMove(tile.index, index);
            }
            break;
        }
    }
    
    diagonalLength = bishopSouthWest[tile.index];
    index = tile.index;
    for(uint8_t i = 0; i < diagonalLength; i++)
    {
        index += 7;
        if(board.b[index] == 0)
        {
            addMove(tile.index, index);
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                addMove(tile.index, index);
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
            addMove(tile.index, index);
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                addMove(tile.index, index);
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.rank - 1; i >= 0; i--) {
        index -= 1;
        if(board.b[index] == 0)
        {
            addMove(tile.index, index);
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                addMove(tile.index, index);
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.file + 1; i < 8; i++) {
        index += 8;
        if(board.b[index] == 0)
        {
            addMove(tile.index, index);
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                addMove(tile.index, index);
            }
            break;
        }
    }
    
    index = tile.index;
    for(int8_t i = tile.file - 1; i >= 0; i--) {
        index -= 8;
        if(board.b[index] == 0)
        {
            addMove(tile.index, index);
        }
        else
        {
            if((board.b[index] < 0) != userToMove)
            {
                addMove(tile.index, index);
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
            bool check = false;
            if(board.castling[2] || board.castling[3]) {
                check = isSquareInCheck(tile, !userToMove);
            }
            if(board.castling[2] && board.b[8] == 0 && board.b[16] == 0 && board.b[24] == 0)
            {
                // queen-side
                if(!isSquareInCheck(Tile(8), !userToMove) && !isSquareInCheck(Tile(16), !userToMove) && !!isSquareInCheck(Tile(24), !userToMove))
                {
                    addMove(32, 16);
                }
            }
            if(board.castling[3] && board.b[48] == 0 && board.b[40] == 0)
            {
                // king-side
                if(!isSquareInCheck(Tile(40), !userToMove) && !isSquareInCheck(Tile(48), !userToMove))
                {
                    addMove(32, 48);
                }
            }
        }
    }
    else
    {
        if(tile.index == 39)
        {
            bool check = false;
            if(board.castling[0] || board.castling[1]) {
                check = isSquareInCheck(tile, !userToMove);
            }
            if(board.castling[0] && board.b[15] == 0 && board.b[23] == 0 && board.b[31] == 0)
            {
                // queen-side
                if(!isSquareInCheck(Tile(15), !userToMove) && !isSquareInCheck(Tile(23), !userToMove) && !isSquareInCheck(Tile(31), !userToMove))
                {
                    addMove(39, 23);
                }
            }
            if(board.castling[1] && board.b[55] == 0 && board.b[47] == 0)
            {
                // king-side
                if(!isSquareInCheck(Tile(47), !userToMove) && !isSquareInCheck(Tile(55), !userToMove))
                {
                    addMove(39, 55);
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
    while(to64 != 0)
    {
        addMove(tile.index, decrementBitboard(to64));
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