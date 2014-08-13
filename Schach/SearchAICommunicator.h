//
//  SearchAICommunicator.h
//  Schach
//
//  Created by Thomas Redding on 7/9/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__SearchAICommunicator__
#define __Schach__SearchAICommunicator__

#include <iostream>
#include "Board.h"
#include "HashTable.h"

class SearchAICommunicator
{
public:
    Board board;
    int from;
    int to;
    float value;
    std::mutex lock;    // does not apply to transpositionTable
    float alpha;
    float beta;
    float depthLeft;
    
    HashTable* transpositionTable;
    
    // 2 = deep sleep, 1 = light sleep, 0 = active
    int levelOfSleep = 2;
};

#endif /* defined(__Schach__SearchAICommunicator__) */
