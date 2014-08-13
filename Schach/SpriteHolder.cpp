//
//  SpriteHolder.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "SpriteHolder.h"

void SpriteHolder::load()
{
    if(!font.loadFromFile(resourcePath() + "sansation.ttf"))
    {
        std::cout << "Failed to Load Font";
    }
    
    int i = 0;
    if (!piece[i].texture.loadFromFile(resourcePath() + "BlackKing.png"))
    {
        std::cout << "FAILED TO LOAD BLACK KING IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 1;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "BlackQueen.png"))
    {
        std::cout << "FAILED TO LOAD BLACK QUEEN IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 1;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "BlackRook.png"))
    {
        std::cout << "FAILED TO LOAD BLACK ROOK IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 5;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "BlackBishop.png"))
    {
        std::cout << "FAILED TO LOAD BLACK BISHOP IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = -5;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "BlackKnight.png"))
    {
        std::cout << "FAILED TO LOAD BLACK KNIGHT IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 1;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "BlackPawn.png"))
    {
        std::cout << "FAILED TO LOAD BLACK PAWN IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 9;
    piece[i].adjustY = 4;
    i++;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "WhitePawn.png"))
    {
        std::cout << "FAILED TO LOAD WHITE PAWN IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 6;
    piece[i].adjustY = 4;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "WhiteKnight.png"))
    {
        std::cout << "FAILED TO LOAD WHITE KNIGHT IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 1;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "WhiteBishop.png"))
    {
        std::cout << "FAILED TO LOAD WHITE BISHOP IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = -5;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "WhiteRook.png"))
    {
        std::cout << "FAILED TO LOAD WHITE ROOK IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 5;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "WhiteQueen.png"))
    {
        std::cout << "FAILED TO LOAD WHITE QUEEN IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 1;
    piece[i].adjustY = 2;
    i++;
    if (!piece[i].texture.loadFromFile(resourcePath() + "WhiteKing.png"))
    {
        std::cout << "FAILED TO LOAD WHITE KING IMAGE";
    }
    piece[i].sprite = sf::Sprite(piece[i].texture);
    piece[i].adjustX = 1;
    piece[i].adjustY = 2;
    
    if (!boardTexture.loadFromFile(resourcePath() + "Board.png"))
    {
        std::cout << "FAILED TO LOAD BOARD IMAGE";
    }
    boardSprite = sf::Sprite(boardTexture);
    
    if(!font.loadFromFile(resourcePath() + "sansation.ttf"))
    {
        std::cout << "FAILED TO LOAD FONT";
    }
    
    if(! preferencesTexture.loadFromFile(resourcePath() + "preferencesIcon.png"))
    {
        std::cout << "FAILED TO LOAD FONT";
    }
    preferencesSprite = sf::Sprite(preferencesTexture);
}