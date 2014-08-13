//
//  Tile.h
//  Schach
//
//  Created by Morgan Redding on 8/12/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef Schach_Tile_h
#define Schach_Tile_h

struct Tile
{
    Tile() {};
    Tile(uint8_t index) : index(index), file(index / 8), rank(index % 8) {};
    uint8_t index, file, rank;
};

#endif
