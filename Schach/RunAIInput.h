//
//  RunAIInput.h
//  Schach
//
//  Created by Thomas Redding on 7/9/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__RunAIInput__
#define __Schach__RunAIInput__

#include <iostream>
#include <vector>
#include "SearchAICommunicator.h"
#include "Board.h"
#include "HashTable.h"

class RunAIInput
{
public:
    Board board;
    std::vector < long > duplicate;
    int cpuTimeLeft;
    std::mutex lock;        // does not apply to transpositionTable or communicator
    int from, to;
    bool shouldAiBeRunning = false;
    std::vector < SearchAICommunicator* > communicator;
    HashTable* transpositionTable;
    bool start;
    bool isAIBlack = true;
    
    // options
    bool useNullMove = true;
    bool useLateMoveReductions = true;
    bool useOpeningBook = true;
    bool useFutilityPruning = true;
    bool useStabilityExtensions = true;
};

#endif /* defined(__Schach__RunAIInput__) */
