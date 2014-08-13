//
//  EndGameMenu.h
//  Schach
//
//  Created by Thomas Redding on 7/9/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__EndGameMenu__
#define __Schach__EndGameMenu__

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SpriteHolder.h>

class EndGameMenu
{
public:
    void open(SpriteHolder* spriteHolderInput);
    void render(sf::RenderWindow* window, sf::Vector2f adjustedPosition, sf::Vector2f absoluteSize, bool aiWhite);
    void frame();
    int mouseDown(float mouseX, float mouseY);
    bool running = false;
private:
    SpriteHolder* spriteHolder;
};

#endif /* defined(__Schach__EndGameMenu__) */
