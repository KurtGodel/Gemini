//
//  SpriteHolder.h
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__SpriteHolder__
#define __Schach__SpriteHolder__

#include <iostream>
#include <Piece.h>
#include "ResourcePath.hpp"

class SpriteHolder
{
public:
    void load();
    Piece piece[13];
    sf::Font font;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    sf::Texture preferencesTexture;
    sf::Sprite preferencesSprite;
};

#endif /* defined(__Schach__SpriteHolder__) */
