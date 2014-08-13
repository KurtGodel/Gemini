//
//  MoveConverter.h
//  Schach
//
//  Created by Thomas Redding on 7/15/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__MoveConverter__
#define __Schach__MoveConverter__

#include <iostream>
#include "Board.h"

class MoveConverter
{
public:
    std::string convertMoveToString(Board* b, int from, int to, int colorOfHuman);
    Board convertFenToBoard(std::string str, bool aiWhite);
private:
    Board board;
    int aiInCheck(int king);
    int humanInCheck(int kingsq);
    void setMoves(int layer);
    bool willMovePutMeInCheck(int king, int from, int to);
    int move[1000];
    int moveLength[2];
};

#endif /* defined(__Schach__MoveConverter__) */
