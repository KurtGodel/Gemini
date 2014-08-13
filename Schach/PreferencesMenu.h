//
//  PreferencesMenu.h
//  Schach
//
//  Created by Thomas Redding on 7/10/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__PreferencesMenu__
#define __Schach__PreferencesMenu__

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "SpriteHolder.h"
#include "NumBox.h"


class PreferencesMenu
{
public:
    PreferencesMenu(int* cpuStartTime, int* humanStartTime, SpriteHolder* spriteHolderInput);
    void open();
    void frame(sf::RenderWindow* window, sf::Vector2f absolutePosition, float absoluteSize, int frameNum);
    void mouseButtonPressed(sf::Event::MouseButtonEvent mouse, float mouseX, float mouseY);
    void keyUp(sf::Keyboard::Key key);
    void keyDown(sf::Keyboard::Key key);
    void mouseMove(sf::Event::MouseMoveEvent mouse);
    void loadPreferences();
    void textEntered(int key);
    void save();
    bool running = false;
    float buttonLeft = 0;
    float buttonTop = 0;
    float buttonWidth = 0;
    float buttonHeight = 0;
private:
    SpriteHolder* spriteHolder;
    int* cpuTimeStarter;
    int* humanTimeStarter;
    void loadLine(std::string str);
    std::vector < NumBox > numBox;
    int focus = -1;
};

#endif /* defined(__Schach__PreferencesMenu__) */
