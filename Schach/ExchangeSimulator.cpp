//
//  ExchangeSimulator.cpp
//  Schach
//
//  Created by Thomas Redding on 7/15/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "ExchangeSimulator.h"

ExchangeSimulator::ExchangeSimulator()
{
    exchangeTable = new float[50000];
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<64; j++)
        {
            exchangeTableIndexArr[i][j] = 1000000000;
        }
    }
    
    for(int i=0; i<50000; i++)
    {
        exchangeTable[i] = -1000000;
    }
    
    // king
    exchangeTableIndexArr[0][0] = 0;
    exchangeTableIndexArr[0][1] = 1;
    // queen
    exchangeTableIndexArr[1][0] = 0;
    exchangeTableIndexArr[1][1] = 2;
    // rook
    exchangeTableIndexArr[2][0] = 0;
    exchangeTableIndexArr[2][1] = 4;
    exchangeTableIndexArr[2][2] = 8;
    // minor
    exchangeTableIndexArr[3][0] = 0;
    exchangeTableIndexArr[3][1] = 12;
    exchangeTableIndexArr[3][2] = 24;
    exchangeTableIndexArr[3][3] = 36;
    // pawn
    exchangeTableIndexArr[4][0] = 0;
    exchangeTableIndexArr[4][1] = 48;
    exchangeTableIndexArr[4][2] = 96;
    // pawn
    exchangeTableIndexArr[5][0] = 0;
    exchangeTableIndexArr[5][1] = 144;
    exchangeTableIndexArr[5][2] = 288;
    // minor
    exchangeTableIndexArr[6][0] = 0;
    exchangeTableIndexArr[6][1] = 432;
    exchangeTableIndexArr[6][2] = 864;
    exchangeTableIndexArr[6][3] = 1296;
    // rook
    exchangeTableIndexArr[7][0] = 0;
    exchangeTableIndexArr[7][1] = 1728;
    exchangeTableIndexArr[7][2] = 3456;
    // queen
    exchangeTableIndexArr[8][0] = 0;
    exchangeTableIndexArr[8][1] = 5184;
    // king
    exchangeTableIndexArr[9][0] = 0;
    exchangeTableIndexArr[9][1] = 10368;
    exchangeTableTurnValue = 20736;
}

void ExchangeSimulator::precomputeExchanges()
{
    int pieceCount[10];
    int maximums[10] = {1, 1, 2, 3, 2, 2, 3, 2, 1, 1};
    for(pieceCount[0] = 0; pieceCount[0] <= maximums[0]; pieceCount[0]++)
    {
        for(pieceCount[1] = 0; pieceCount[1] <= maximums[1]; pieceCount[1]++)
        {
            for(pieceCount[2] = 0; pieceCount[2] <= maximums[2]; pieceCount[2]++)
            {
                for(pieceCount[3] = 0; pieceCount[3] <= maximums[3]; pieceCount[3]++)
                {
                    for(pieceCount[4] = 0; pieceCount[4] <= maximums[4]; pieceCount[4]++)
                    {
                        for(pieceCount[5] = 0; pieceCount[5] <= maximums[5]; pieceCount[5]++)
                        {
                            for(pieceCount[6] = 0; pieceCount[6] <= maximums[6]; pieceCount[6]++)
                            {
                                for(pieceCount[7] = 0; pieceCount[7] <= maximums[7]; pieceCount[7]++)
                                {
                                    for(pieceCount[8] = 0; pieceCount[8] <= maximums[8]; pieceCount[8]++)
                                    {
                                        for(pieceCount[9] = 0; pieceCount[9] <= maximums[9]; pieceCount[9]++)
                                        {
                                            precomputeSingleExchange(&pieceCount[0]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ExchangeSimulator::precomputeSingleExchange(int* pieceCount)
{
    for(int i=0; i<10; i++)
    {
        spaceControl[0][0][i] = pieceCount[i];
    }
    board.b[0] = 6;
    int key = spaceControlToIndex(0, 0, board.b[0]);
    int value = pieceSafe(0, 0) + exchangeValue[4];
    if(key < 10000000)
    {
        exchangeTable[key] = value;
    }
    
    board.b[0] = -6;
    key = spaceControlToIndex(0, 0, board.b[0]);
    value = pieceSafe(0, 0) + exchangeValue[4];
    if(key < 10000000)
    {
        exchangeTable[key] = value;
    }
}

float ExchangeSimulator::getPrecomputedValue(int* attackingPiece, int pieceOnSquare)
{
    int index = spaceControlToIndex(attackingPiece, pieceOnSquare);
    if(index < 1000000)
    {
        return fminf(exchangeTable[index]-fabsf(pieceToValue(pieceOnSquare)), 0);
    }
    return -1000000;
}

int ExchangeSimulator::spaceControlToIndex(int* attackingPiece, int pieceOnSquare)
{
    int index = 0;
    for(int i=0; i<10; i++)
    {
        index += exchangeTableIndexArr[i][attackingPiece[i]];
    }
    if(pieceOnSquare > 0)
    {
        index += exchangeTableTurnValue;
    }
    return index;
}

int ExchangeSimulator::spaceControlToIndex(int layer, int sq, int piece)
{
    int index = 0;
    for(int i=0; i<10; i++)
    {
        index += exchangeTableIndexArr[i][spaceControl[layer][sq][i]];
    }
    if(piece > 0)
    {
        index += exchangeTableTurnValue;
    }
    return index;
}

ExchangeSimulator::~ExchangeSimulator()
{
    delete[] exchangeTable;
}

float ExchangeSimulator::pieceSafe(int sq, int layer)
{
    if(board.b[sq] == 0)
    {
        return 0;
    }
    else if(board.b[sq]>0)
    {
        // cpu's piece
        int cpuX = 0;
        int cpuCounter = 0;
        int humanX = 0;
        int humanCounter = 0;
        float currentOnSquare = pieceToValue(board.b[sq]);
        exchangeOption[0] = 0;
        exchangeOptionLength = 1;
        for(int i=0;i<100;i++)
        {
            if(i%2==0)
            {
                // human's turn
                for(humanX=humanX;humanX<5;humanX++)
                {
                    if(spaceControl[layer][sq][4-humanX]-humanCounter>0)
                    {
                        exchangeOption[exchangeOptionLength] = exchangeOption[exchangeOptionLength-1]-currentOnSquare;
                        exchangeOptionLength++;
                        currentOnSquare = exchangeValue[humanX];
                        humanCounter++;
                        break;
                    }
                    humanCounter = 0;
                }
                if(humanX==5)
                {
                    // has run out of pieces
                    break;
                }
            }
            else
            {
                // cpu's turn
                for(cpuX=cpuX;cpuX<5;cpuX++)
                {
                    if(spaceControl[layer][sq][5+cpuX]-cpuCounter>0)
                    {
                        exchangeOption[exchangeOptionLength] = exchangeOption[exchangeOptionLength-1]+currentOnSquare;
                        exchangeOptionLength++;
                        currentOnSquare = exchangeValue[cpuX];
                        cpuCounter++;
                        break;
                    }
                    cpuCounter = 0;
                }
                if(cpuX==5)
                {
                    // has run out of pieces
                    break;
                }
            }
        }
        return exchangeTree(0);
    }
    else
    {
        // human piece
        int i;
        int cpuX = 0;
        int cpuCounter = 0;
        int humanX = 0;
        int humanCounter = 0;
        float currentOnSquare = abs(pieceToValue((int) board.b[sq]));
        exchangeOption[0] = 0;
        exchangeOptionLength = 1;
        for(i=0;i<100;i++)
        {
            if(i%2==1)
            {
                // human's turn
                for(humanX=humanX;humanX<5;humanX++)
                {
                    if(spaceControl[layer][sq][4-humanX]-humanCounter>0)
                    {
                        exchangeOption[exchangeOptionLength] = exchangeOption[exchangeOptionLength-1]+currentOnSquare;
                        exchangeOptionLength++;
                        currentOnSquare = exchangeValue[humanX];
                        humanCounter++;
                        break;
                    }
                    humanCounter = 0;
                }
                if(humanX==5)
                {
                    // has run out of pieces
                    break;
                }
            }
            else
            {
                // cpu's turn
                for(cpuX=cpuX;cpuX<5;cpuX++)
                {
                    if(spaceControl[layer][sq][5+cpuX]-cpuCounter>0)
                    {
                        exchangeOption[exchangeOptionLength] = exchangeOption[exchangeOptionLength-1]-currentOnSquare;
                        exchangeOptionLength++;
                        currentOnSquare = exchangeValue[cpuX];
                        cpuCounter++;
                        break;
                    }
                    cpuCounter = 0;
                }
                if(cpuX==5)
                {
                    // has run out of pieces
                    break;
                }
            }
        }
        return exchangeTree(0);
    }
}

int ExchangeSimulator::pieceToValue(int num)
{
    if(num == 0)
    {
        return 0;
    }
    if(num==-6)
    {
        return -20000;
    }
    if(num==-5)
    {
        return -900;
    }
    else if(num==-4)
    {
        return -500;
    }
    else if(num==-3)
    {
        return -325;
    }
    else if(num==-2)
    {
        return -325;
    }
    else if(num==-1)
    {
        return -100;
    }
    else if(num==1)
    {
        return 100;
    }
    else if(num==2)
    {
        return 325;
    }
    else if(num==3)
    {
        return 325;
    }
    else if(num==4)
    {
        return 500;
    }
    else if(num==5)
    {
        return 900;
    }
    else if(num==6)
    {
        return 20000;
    }
    return 0;
}

float ExchangeSimulator::exchangeTree(int layer)
{
    if(layer==exchangeOptionLength-1)
    {
        return exchangeOption[exchangeOptionLength-1];
    }
    float alternative = exchangeTree(layer+1);
    if(layer%2==0)
    {
        if(alternative<exchangeOption[layer])
        {
            return alternative;
        }
        else
        {
            return exchangeOption[layer];
        }
    }
    else
    {
        if(alternative>exchangeOption[layer])
        {
            return alternative;
        }
        else
        {
            return exchangeOption[layer];
        }
    }
    return 0;
}
