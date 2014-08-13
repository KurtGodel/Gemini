//
//  Move.h
//  Schach
//
//  Created by Morgan Redding on 8/10/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__Move__
#define __Schach__Move__

struct Move {
    Move() {};
    Move(int f, int t) : from(f), to(t) {};
    uint16_t from, to;
    std::string toString() {
        std::string r = std::to_string(from);
        r += " -> ";
        r += std::to_string(to);
        return r;
    }
};

#endif /* defined(__Schach__Move__) */
