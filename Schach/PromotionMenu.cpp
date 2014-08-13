//
//  PromotionMenu.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "PromotionMenu.h"

void PromotionMenu::open(SpriteHolder* spriteHolderInput, int f, int t)
{
    from = f;
    to = t;
    running = true;
    spriteHolder = spriteHolderInput;
}

void PromotionMenu::render(sf::RenderWindow* window, sf::Vector2f adjustedPosition, sf::Vector2f adjustedSize, bool aiWhite)
{
    sf::RectangleShape rect;
    rect.setSize(adjustedSize);
    rect.setPosition(adjustedPosition);
    rect.setFillColor(sf::Color(0, 0, 0, 128));
    (*window).draw(rect);
    
    if(aiWhite)
    {
        (*spriteHolder).piece[1].sprite.setPosition(sf::Vector2f(adjustedPosition.x+adjustedSize.x*0.05, adjustedPosition.y+adjustedSize.y*0.05));
        (*spriteHolder).piece[1].sprite.setScale(adjustedSize.x/800, adjustedSize.y/800);
        (*window).draw((*spriteHolder).piece[1].sprite);
        
        (*spriteHolder).piece[2].sprite.setPosition(sf::Vector2f(adjustedPosition.x+adjustedSize.x*0.10, adjustedPosition.y+adjustedSize.y*0.55));
        (*spriteHolder).piece[2].sprite.setScale(adjustedSize.x/800, adjustedSize.y/800);
        (*window).draw((*spriteHolder).piece[2].sprite);
        
        (*spriteHolder).piece[3].sprite.setPosition(sf::Vector2f(adjustedPosition.x+adjustedSize.x*0.55, adjustedPosition.y+adjustedSize.y*0.05));
        (*spriteHolder).piece[3].sprite.setScale(adjustedSize.x/800, adjustedSize.y/800);
        (*window).draw((*spriteHolder).piece[3].sprite);
        
        (*spriteHolder).piece[4].sprite.setPosition(sf::Vector2f(adjustedPosition.x+adjustedSize.x*0.55, adjustedPosition.y+adjustedSize.y*0.55));
        (*spriteHolder).piece[4].sprite.setScale(adjustedSize.x/800, adjustedSize.y/800);
        (*window).draw((*spriteHolder).piece[4].sprite);
    }
    else
    {
        (*spriteHolder).piece[11].sprite.setPosition(sf::Vector2f(adjustedPosition.x+adjustedSize.x*0.05, adjustedPosition.y+adjustedSize.y*0.05));
        (*spriteHolder).piece[11].sprite.setScale(adjustedSize.x/800, adjustedSize.y/800);
        (*window).draw((*spriteHolder).piece[11].sprite);
        
        (*spriteHolder).piece[10].sprite.setPosition(sf::Vector2f(adjustedPosition.x+adjustedSize.x*0.10, adjustedPosition.y+adjustedSize.y*0.55));
        (*spriteHolder).piece[10].sprite.setScale(adjustedSize.x/800, adjustedSize.y/800);
        (*window).draw((*spriteHolder).piece[10].sprite);
        
        (*spriteHolder).piece[9].sprite.setPosition(sf::Vector2f(adjustedPosition.x+adjustedSize.x*0.55, adjustedPosition.y+adjustedSize.y*0.05));
        (*spriteHolder).piece[9].sprite.setScale(adjustedSize.x/800, adjustedSize.y/800);
        (*window).draw((*spriteHolder).piece[9].sprite);
        
        (*spriteHolder).piece[8].sprite.setPosition(sf::Vector2f(adjustedPosition.x+adjustedSize.x*0.55, adjustedPosition.y+adjustedSize.y*0.55));
        (*spriteHolder).piece[8].sprite.setScale(adjustedSize.x/800, adjustedSize.y/800);
        (*window).draw((*spriteHolder).piece[8].sprite);
    }
}

int PromotionMenu::mouseDown(float mouseX, float mouseY)
{
    if(mouseX<0.5)
    {
        if(mouseY<0.5)
        {
            return 5;
        }
        else
        {
            return 4;
        }
    }
    else
    {
        if(mouseY<0.5)
        {
            return 3;
        }
        else
        {
            return 2;
        }
    }
}

void PromotionMenu::frame()
{
    //
}