//
//  PromotionMenu.h
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__PromotionMenu__
#define __Schach__PromotionMenu__

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SpriteHolder.h>

class PromotionMenu
{
public:
    void open(SpriteHolder* spriteHolderInput, int from, int to);
    void render(sf::RenderWindow* window, sf::Vector2f adjustedPosition, sf::Vector2f adjustedSize, bool aiWhite);
    void frame();
    int mouseDown(float mouseX, float mouseY);
    bool running = false;
    int from, to;
private:
    SpriteHolder* spriteHolder;
};

#endif /* defined(__Schach__PromotionMenu__) */
