//
//  EvaluatorTracker.h
//  Schach
//
//  Created by Thomas Redding on 7/18/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__EvaluatorTracker__
#define __Schach__EvaluatorTracker__

#include <iostream>
#include "Board.h"

class EvaluatorTracker
{
public:
    void set(Board* board);
    int makeMove(int from, int to, Board* board);
    void undoMove(int from, int to, Board* board, int sto);
    void validate(Board* board, int from, int to, int sto);
    int cpuPawns = 0;
    int cpuKnights = 0;
    int cpuBishops = 0;
    int cpuRooks = 0;
    int cpuQueens = 0;
    int humanPawns = 0;
    int humanKnights = 0;
    int humanBishops = 0;
    int humanRooks = 0;
    int humanQueens = 0;
    
    int cpuKing = -1;
    int humanKing = -1;
};

#endif /* defined(__Schach__EvaluatorTracker__) */
