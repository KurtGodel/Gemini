//
//  EndGameMenu.cpp
//  Schach
//
//  Created by Thomas Redding on 7/9/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "EndGameMenu.h"

void EndGameMenu::open(SpriteHolder* spriteHolderInput)
{
    running = true;
    spriteHolder = spriteHolderInput;
}

void EndGameMenu::render(sf::RenderWindow* window, sf::Vector2f adjustedPosition, sf::Vector2f adjustedSize, bool aiWhite)
{
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(adjustedSize.x, adjustedSize.y));
    rect.setPosition(adjustedPosition);
    rect.setFillColor(sf::Color(0, 0, 0, 128));
    (*window).draw(rect);
    
    std::string str = "Game Over";
    sf::Text text(str, (*spriteHolder).font, adjustedSize.y/10);
    text.setPosition(adjustedPosition.x+adjustedSize.x/2-text.getGlobalBounds().width/2, adjustedPosition.y+adjustedSize.y/2-text.getGlobalBounds().height/2-adjustedSize.y/40);
    text.setColor(sf::Color(255, 0, 0));
    (*window).draw(text);
}

int EndGameMenu::mouseDown(float mouseX, float mouseY)
{
    return 0;
}

void EndGameMenu::frame()
{
    //
}