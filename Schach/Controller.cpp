//
//  Controller.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "Controller.h"

Controller::Controller(RunAIInput* gameAiCommunicator)
:game(&spriteHolder, gameAiCommunicator),
preferencesMenu(&game.cpuStartTime, &game.humanStartTime, &spriteHolder)
{
    spriteHolder.load();
    stretchFactor.x = 1;
    stretchFactor.y = 1;
}

void Controller::mouseDown(sf::Event::MouseButtonEvent mouse)
{
    if(mouse.x >= preferencesMenu.buttonLeft && mouse.y >= preferencesMenu.buttonTop && mouse.x <= preferencesMenu.buttonLeft+preferencesMenu.buttonWidth && mouse.y <= preferencesMenu.buttonTop+preferencesMenu.buttonHeight)
    {
        if(preferencesMenu.running)
        {
            preferencesMenu.save();
            preferencesMenu.running = false;
        }
        else
        {
            preferencesMenu.open();
        }
    }
    else if(mouse.x >= newGameButtonPosition.x && mouse.y >= newGameButtonPosition.y&& mouse.x <= newGameButtonPosition.x+newGameButtonSize.x && mouse.y <= newGameButtonPosition.y+newGameButtonSize.y)
    {
        game.newGame();
    }
    else if(preferencesMenu.running)
    {
        preferencesMenu.mouseButtonPressed(mouse, (mouse.x-boardPosition.x)/boardSize, (mouse.x-boardPosition.y)/boardSize);
    }
    else
    {
        game.mouseDown(mouse);
    }
}

void Controller::mouseUp(sf::Event::MouseButtonEvent mouse)
{
    if(!preferencesMenu.running)
    {
        game.mouseUp(mouse);
    }
}

void Controller::mouseMove(sf::Event::MouseMoveEvent mouse)
{
    if(preferencesMenu.running)
    {
        preferencesMenu.mouseMove(mouse);
    }
    else
    {
        game.mouseMove(mouse);
    }
}

void Controller::textEntered(int key)
{
    if(preferencesMenu.running)
    {
        preferencesMenu.textEntered(key);
    }
}

void Controller::keyDown(sf::Keyboard::Key key)
{
    if(preferencesMenu.running)
    {
        preferencesMenu.keyDown(key);
    }
    else
    {
        game.keyDown(key);
    }
}

void Controller::keyUp(sf::Keyboard::Key key)
{
    if(preferencesMenu.running)
    {
        preferencesMenu.keyUp(key);
    }
    else
    {
        game.keyUp(key);
    }
}

void Controller::resized(sf::Event::SizeEvent eventInfo)
{
    stretchFactor.x = eventInfo.width/800.0;
    stretchFactor.y = eventInfo.height/600.0;
}

void Controller::frame(sf::RenderWindow* window)
{
    if(window->getSize().x > window->getSize().y)
    {
        boardSize = window->getSize().y;
        preferencesMenu.buttonLeft = boardSize+10;
        preferencesMenu.buttonTop = 10;
        preferencesMenu.buttonWidth = 50;
        preferencesMenu.buttonHeight = 50;
        newGameButtonPosition.x = boardSize+10;
        newGameButtonPosition.y = 70;
        newGameButtonSize.x = 80;
        newGameButtonSize.y = 25;
    }
    else
    {
        boardSize = window->getSize().x;
        preferencesMenu.buttonLeft = 0;
        preferencesMenu.buttonTop = 0;
        preferencesMenu.buttonWidth = 0;
        preferencesMenu.buttonHeight = 0;
        newGameButtonPosition.x = 0;
        newGameButtonPosition.y = 0;
        newGameButtonSize.x = 0;
        newGameButtonSize.y = 0;
    }
    
    sf::Text text("", spriteHolder.font, boardSize/20);
    text.setColor(sf::Color(255, 0, 0));
    text.setString(timeToString(game.cpuTimeLeft));
    text.setPosition(650, 250);
    window->draw(text);
    text.setString(timeToString(game.humanTimeLeft));
    text.setPosition(650, 350);
    window->draw(text);
    
    spriteHolder.preferencesSprite.setScale(preferencesMenu.buttonWidth/512, preferencesMenu.buttonHeight/512);
    spriteHolder.preferencesSprite.setPosition(preferencesMenu.buttonLeft, preferencesMenu.buttonTop);
    window->draw(spriteHolder.preferencesSprite);
    
    sf::RectangleShape rect;
    rect.setSize(newGameButtonSize);
    rect.setPosition(newGameButtonPosition);
    rect.setFillColor(sf::Color(128, 128, 128));
    window->draw(rect);
    
    text.setString("New Game");
    text.setPosition(newGameButtonPosition.x+newGameButtonSize.x*0.04, newGameButtonPosition.y+newGameButtonSize.y*0.08);
    text.scale(newGameButtonSize.y/50, newGameButtonSize.y/50);
    window->draw(text);
    
    
    game.frame(window, boardPosition, boardSize, stretchFactor, !preferencesMenu.running);
    if(preferencesMenu.running)
    {
        preferencesMenu.frame(window, boardPosition, boardSize, frameNum);
    }
    frameNum++;
}

std::string Controller::timeToString(int ms)
{
    int time = (ms+999)/1000;
    int minutes = time/60;
    int seconds = time-60*minutes;
    std::string rtn = "";
    if(minutes>0)
    {
        rtn += std::to_string(minutes);
        rtn += ":";
        if(seconds > 9)
        {
            rtn += std::to_string(seconds);
        }
        else if(seconds > 0)
        {
            rtn += "0" + std::to_string(seconds);
        }
        else
        {
            rtn += "00";
        }
    }
    else
    {
        rtn += std::to_string(seconds);
    }
    return rtn;
}