//
//  Controller.h
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__Controller__
#define __Schach__Controller__

#include <iostream>
#include <Game.h>
#include <SpriteHolder.h>

class Controller
{
public:
    Controller(RunAIInput* gameAiCommunicator);
    void mouseDown(sf::Event::MouseButtonEvent mouse);
    void mouseUp(sf::Event::MouseButtonEvent mouse);
    void mouseMove(sf::Event::MouseMoveEvent mouse);
    void keyDown(sf::Keyboard::Key key);
    void keyUp(sf::Keyboard::Key key);
    void resized(sf::Event::SizeEvent eventInfo);
    void frame(sf::RenderWindow* window);
    void textEntered(int key);
    Game game;
private:
    sf::Vector2f boardPosition;
    sf::Vector2f newGameButtonPosition;
    sf::Vector2f newGameButtonSize;
    float boardSize;
    PreferencesMenu preferencesMenu;
    int frameNum = 0;
    sf::Vector2f stretchFactor;
    SpriteHolder spriteHolder;
    std::string timeToString(int ms);
};

#endif /* defined(__Schach__Controller__) */
