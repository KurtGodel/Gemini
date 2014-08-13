//
//  EvaluatorTracker.cpp
//  Schach
//
//  Created by Thomas Redding on 7/18/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "EvaluatorTracker.h"

void EvaluatorTracker::set(Board* board)
{
    cpuKing = -1;
    humanKing = -1;
    cpuPawns = 0;
    cpuKnights = 0;
    cpuBishops = 0;
    cpuRooks = 0;
    cpuQueens = 0;
    humanPawns = 0;
    humanKnights = 0;
    humanBishops = 0;
    humanRooks = 0;
    humanQueens = 0;
    for(int i=0; i<64; i++)
    {
        if(board->b[i] > 0)
        {
            if(board->b[i] == 1)
            {
                cpuPawns++;
            }
            else if(board->b[i] == 2)
            {
                cpuKnights++;
            }
            else if(board->b[i] == 3)
            {
                cpuBishops++;
            }
            else if(board->b[i] == 4)
            {
                cpuRooks++;
            }
            else if(board->b[i] == 5)
            {
                cpuQueens++;
            }
            else if(board->b[i] == 6)
            {
                cpuKing = i;
            }
        }
        else if(board->b[i] < 0)
        {
            if(board->b[i] == -1)
            {
                humanPawns++;
            }
            else if(board->b[i] == -2)
            {
                humanKnights++;
            }
            else if(board->b[i] == -3)
            {
                humanBishops++;
            }
            else if(board->b[i] == -4)
            {
                humanRooks++;
            }
            else if(board->b[i] == -5)
            {
                humanQueens++;
            }
            else if(board->b[i] == -6)
            {
                humanKing = i;
            }
        }
    }
}

int EvaluatorTracker::makeMove(int from, int to, Board* board)
{
    int rtn = 0;
    // TOOD: board->makeMove(from, to);
    if(board->b[from%100] > 0)
    {
        // cpu
        if(from>=100)
        {
            // promotion
            cpuPawns--;
            int newPiece = from/100;
            if(newPiece == 2)
            {
                cpuKnights++;
            }
            if(newPiece == 3)
            {
                cpuBishops++;
            }
            if(newPiece == 4)
            {
                cpuRooks++;
            }
            if(newPiece == 5)
            {
                cpuQueens++;
            }
        }
        
        if(board->b[from%100] == 6)
        {
            cpuKing = to;
        }
        
        if(board->b[from%100]==1 && board->b[to]==0 && (to-from==7 || to-from==-9))
        {
            // En passant
            rtn = -1;
            humanPawns--;
        }
        else if(board->b[to] == -1)
        {
            rtn = -1;
            humanPawns--;
        }
        else if(board->b[to] == -2)
        {
            rtn = -2;
            humanKnights--;
        }
        else if(board->b[to] == -3)
        {
            rtn = -3;
            humanBishops--;
        }
        else if(board->b[to] == -4)
        {
            rtn = -4;
            humanRooks--;
        }
        else if(board->b[to] == -5)
        {
            rtn = -5;
            humanQueens--;
        }
    }
    else
    {
        // human
        if(from>=100)
        {
            // promotion
            humanPawns--;
            int newPiece = from/100;
            if(newPiece == 2)
            {
                humanKnights++;
            }
            if(newPiece == 3)
            {
                humanBishops++;
            }
            if(newPiece == 4)
            {
                humanRooks++;
            }
            if(newPiece == 5)
            {
                humanQueens++;
            }
        }
        
        if(board->b[from%100] == -6)
        {
            humanKing = to;
        }
        
        if(board->b[from%100]==1 && board->b[to]==0 && (to-from==7 || to-from==-9))
        {
            // En passant
            rtn = 1;
            cpuPawns--;
        }
        else if(board->b[to] == 1)
        {
            rtn = 1;
            cpuPawns--;
        }
        else if(board->b[to] == 2)
        {
            rtn = 2;
            cpuKnights--;
        }
        else if(board->b[to] == 3)
        {
            rtn = 3;
            cpuBishops--;
        }
        else if(board->b[to] == 4)
        {
            rtn = 4;
            cpuRooks--;
        }
        else if(board->b[to] == 5)
        {
            rtn = 5;
            cpuQueens--;
        }
    }
    return rtn;
}

void EvaluatorTracker::undoMove(int from, int to, Board* board, int sto)
{
    if(board->b[from%100] > 0)
    {
        // cpu
        if(from >= 100)
        {
            // promotion
            cpuPawns++;
            int newPiece = from/100;
            if(newPiece == 2)
            {
                cpuKnights--;
            }
            else if(newPiece == 3)
            {
                cpuBishops--;
            }
            else if(newPiece == 4)
            {
                cpuRooks--;
            }
            else if(newPiece == 5)
            {
                cpuQueens--;
            }
        }
        
        if(board->b[from%100] == 6)
        {
            cpuKing = from;
        }
        
        if(sto == -1)
        {
            humanPawns++;
        }
        else if(sto == -2)
        {
            humanKnights++;
        }
        else if(sto == -3)
        {
            humanBishops++;
        }
        else if(sto == -4)
        {
            humanRooks++;
        }
        else if(sto == -5)
        {
            humanQueens++;
        }
    }
    else
    {
        // human
        if(from >= 100)
        {
            // promotion
            humanPawns++;
            int newPiece = from/100;
            if(newPiece == 2)
            {
                humanKnights--;
            }
            else if(newPiece == 3)
            {
                humanBishops--;
            }
            else if(newPiece == 4)
            {
                humanRooks--;
            }
            else if(newPiece == 5)
            {
                humanQueens--;
            }
        }
        
        if(board->b[from%100] == -6)
        {
            humanKing = from;
        }
        
        if(sto == 1)
        {
            cpuPawns++;
        }
        else if(sto == 2)
        {
            cpuKnights++;
        }
        else if(sto == 3)
        {
            cpuBishops++;
        }
        else if(sto == 4)
        {
            cpuRooks++;
        }
        else if(sto == 5)
        {
            cpuQueens++;
        }
    }
    
    validate(board, from, to, sto);
}

void EvaluatorTracker::validate(Board* board, int from, int to, int sto)
{
    int cpuPawnsA = 0;
    int cpuKnightsA = 0;
    int cpuBishopsA = 0;
    int cpuRooksA = 0;
    int cpuQueensA = 0;
    int humanPawnsA = 0;
    int humanKnightsA = 0;
    int humanBishopsA = 0;
    int humanRooksA = 0;
    int humanQueensA = 0;
    for(int i=0; i<64; i++)
    {
        if(board->b[i] > 0)
        {
            if(board->b[i] == 1)
            {
                cpuPawnsA++;
            }
            else if(board->b[i] == 2)
            {
                cpuKnightsA++;
            }
            else if(board->b[i] == 3)
            {
                cpuBishopsA++;
            }
            else if(board->b[i] == 4)
            {
                cpuRooksA++;
            }
            else if(board->b[i] == 5)
            {
                cpuQueensA++;
            }
        }
        else if(board->b[i] < 0)
        {
            if(board->b[i] == -1)
            {
                humanPawnsA++;
            }
            else if(board->b[i] == -2)
            {
                humanKnightsA++;
            }
            else if(board->b[i] == -3)
            {
                humanBishopsA++;
            }
            else if(board->b[i] == -4)
            {
                humanRooksA++;
            }
            else if(board->b[i] == -5)
            {
                humanQueensA++;
            }
        }
    }
}