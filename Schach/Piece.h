//
//  Piece.h
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__Piece__
#define __Schach__Piece__

#include <iostream>
#include <SFML/Graphics.hpp>

class Piece
{
public:
    sf::Sprite sprite;
    sf::Texture texture;
    float adjustX;
    float adjustY;
};

#endif /* defined(__Schach__Piece__) */
