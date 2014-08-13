//
//  OpeningMove.h
//  Schach
//
//  Created by Thomas Redding on 7/2/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__OpeningMove__
#define __Schach__OpeningMove__

#include <iostream>
#include <vector>

class OpeningMove
{
public:
    int zobristKey = -1;
    
    // from, to, weight, from, to, weight, ...
    std::vector < int > move;
};

#endif /* defined(__Schach__OpeningMove__) */
