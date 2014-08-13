//
//  ExchangeSimulator.h
//  Schach
//
//  Created by Thomas Redding on 7/15/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__ExchangeSimulator__
#define __Schach__ExchangeSimulator__

#include <iostream>
#include <cmath>
#include "Board.h"

class ExchangeSimulator
{
public:
    ExchangeSimulator();
    ~ExchangeSimulator();
    void precomputeExchanges();
    float getPrecomputedValue(int* attackingPiece, int pieceOnSquare);
    
private:
    int spaceControlToIndex(int* attackingPiece, int pieceOnSquare);
    int spaceControlToIndex(int layer, int sq, int piece);
    void precomputeSingleExchange(int* pieceCount);
    float pieceSafe(int sq, int layer);
    int pieceToValue(int num);
    float exchangeTree(int layer);
    
    Board board;
    float* exchangeTable;
    int exchangeTableIndexArr[10][64];
    int exchangeTableTurnValue;
    int spaceControl[1][64][10];
    float exchangeValue[5] = {100,325,500,900,20000};
    float exchangeOption[100];
    int exchangeOptionLength = 0;
};

#endif /* defined(__Schach__ExchangeSimulator__) */
