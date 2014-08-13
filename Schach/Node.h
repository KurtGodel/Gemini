//
//  Node.h
//  Schach
//
//  Created by Morgan Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__Node__
#define __Schach__Node__

#include <iostream>

struct Node {
    int bestMove = -1;
    int value;
    int betaSearchedAt;
    bool searchCompleted;
    float depthSearched;            // 100 indicates three-move draw thingy
};

#endif /* defined(__Schach__Node__) */
