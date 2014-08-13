//
//  NumBox.h
//  Schach
//
//  Created by Thomas Redding on 7/5/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__NumBox__
#define __Schach__NumBox__

#include <iostream>
#include <SFML/Graphics.hpp>

class NumBox
{
public:
    NumBox(sf::Vector2f pos, sf::Vector2f siz);
    void render(sf::RenderWindow* window, sf::Vector2f scale, sf::Vector2f offset, int frame, sf::Font *fontToUse);
    void textEntered(int key);
    void keyPressed(sf::Keyboard::Key key);
    void focus(sf::Event::MouseButtonEvent mouse);
    void focus();
    void unfocus();
    void setPosition(sf::Vector2f vect);
    void setPosition(float x, float y);
    void setSize(sf::Vector2f vect);
    void setSize(float x, float y);
    bool mouseButtonPressed(sf::Event::MouseButtonEvent mouse);
    void mouseMove(sf::Event::MouseMoveEvent mouse);
    void mouseUp(sf::Event::MouseButtonEvent mouse);
    void keyReleased(int key);
    std::string getValue();
    void setValue(std::string newString);
private:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2f absolutePosition;
    sf::Vector2f absoluteSize;
    std::string str;
    sf::Font* font;
    int cursorSpot = 0;
    std::string UnicodeToString(int key);
    bool focused = false;
    int maxCharacterLength = 10;
    sf::Vector2u windowSize;
    bool leftShift = false;
    bool rightShift = false;
    bool leftCommand = false;
    bool rightCommand = false;
    int highlightStart = -1;
    bool mouseDown = false;
};

#endif /* defined(__Schach__NumBox__) */