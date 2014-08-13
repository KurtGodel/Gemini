//
//  NumBox.cpp
//  Schach
//
//  Created by Thomas Redding on 7/5/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "NumBox.h"

NumBox::NumBox(sf::Vector2f pos, sf::Vector2f siz)
{
    position.x = pos.x;
    position.y = pos.y;
    size.x = siz.x;
    size.y = siz.y;
    windowSize.x = 0;
    windowSize.y = 0;
    str = "";
}

void NumBox::setPosition(sf::Vector2f vect)
{
    position.x = vect.x;
    position.y = vect.y;
}

void NumBox::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

bool NumBox::mouseButtonPressed(sf::Event::MouseButtonEvent mouse)
{
    if(mouse.x>absolutePosition.x && mouse.x<absolutePosition.x+absoluteSize.x && mouse.y>absolutePosition.y && mouse.y<absolutePosition.y+absoluteSize.y)
    {
        mouseDown = true;
        focus(mouse);
        return true;
    }
    unfocus();
    return false;
}

void NumBox::mouseUp(sf::Event::MouseButtonEvent mouse)
{
    mouseDown = false;
}

void NumBox::mouseMove(sf::Event::MouseMoveEvent mouse)
{
    if(mouseDown)
    {
        if(highlightStart == -1)
        {
            highlightStart = cursorSpot;
        }
        
        float mouseX = (mouse.x - absolutePosition.x)/absoluteSize.x;
        if(mouseX>=0 && mouseX<=1)
        {
            // mouse is in textbox
            if(mouse.x<absolutePosition.y)
            {
                cursorSpot = 0;
            }
            else if(mouse.y>absolutePosition.y+absoluteSize.y)
            {
                cursorSpot = 0;
            }
            else
            {
                sf::Text textB("", *font, absoluteSize.y);
                float textBWidth;
                int i;
                for(i=0; i<str.length(); i++)
                {
                    textB.setString(str.substr(0, i));
                    textBWidth = textB.getGlobalBounds().width;
                    if(textBWidth+absolutePosition.x>mouse.x)
                    {
                        break;
                    }
                }
                cursorSpot = i;
            }
        }
        else if(mouseX<=0)
        {
            cursorSpot = 0;
        }
        else if(mouseX<=0)
        {
            cursorSpot = str.length();
        }
    }
}

void NumBox::setSize(sf::Vector2f vect)
{
    size.x = vect.x;
    size.y = vect.y;
}

void NumBox::setSize(float x, float y)
{
    size.x = x;
    size.y = y;
}

void NumBox::render(sf::RenderWindow* window, sf::Vector2f scale, sf::Vector2f offset, int frame, sf::Font *fontToUse)
{
    font = fontToUse;
    // renderbox
    windowSize = (*window).getSize();
    sf::RectangleShape renderObject;
    absoluteSize.x = size.x*scale.x;
    absoluteSize.y = size.y*scale.y;
    absolutePosition.x = position.x*scale.x+offset.x-absoluteSize.x/2;
    absolutePosition.y = position.y*scale.y+offset.y-absoluteSize.y/2;
    
    maxCharacterLength = (int) (absoluteSize.x * 1.5 /absoluteSize.y);
    
    renderObject.setPosition(absolutePosition);
    renderObject.setSize(absoluteSize);
    (*window).draw(renderObject);
    
    // render text
    sf::Text text(str, *font, absoluteSize.y);
    text.setPosition(absolutePosition.x, absolutePosition.y-absoluteSize.y*0.15);
    text.setColor(sf::Color::Black);
    (*window).draw(text);
    
    // render cursor & highlighting
    if(highlightStart == -1)
    {
        // render cursor
        if(focused && frame%40<20)
        {
            sf::Text textB(str.substr(0, cursorSpot), *font, absoluteSize.y);
            float textBWidth = textB.getGlobalBounds().width;
            sf::VertexArray line(sf::LinesStrip, 2);
            line[0].position = sf::Vector2f(absolutePosition.x+textBWidth+2, absolutePosition.y);
            line[0].color = sf::Color(0, 0, 0);
            line[1].position = sf::Vector2f(absolutePosition.x+textBWidth+2, absolutePosition.y+absoluteSize.y);
            line[1].color = sf::Color(0, 0, 0);
            (*window).draw(line);
        }
    }
    else
    {
        sf::Color highlightColor(128, 128, 255);
        // render highlighting
        if(focused)
        {
            sf::Text textB(str.substr(0, cursorSpot), *font, absoluteSize.y);
            float cursorX = textB.getGlobalBounds().width;
            textB.setString(str.substr(0, highlightStart));
            float highlightX = textB.getGlobalBounds().width;
            
            if(cursorX < highlightX)
            {
                sf::RectangleShape rect(sf::Vector2f(highlightX-cursorX, absoluteSize.y));
                rect.setPosition(absolutePosition.x+cursorX, absolutePosition.y);
                rect.setFillColor(highlightColor);
                (*window).draw(rect);
            }
            else
            {
                sf::RectangleShape rect(sf::Vector2f(cursorX-highlightX, absoluteSize.y));
                rect.setPosition(absolutePosition.x+highlightX, absolutePosition.y);
                rect.setFillColor(highlightColor);
                (*window).draw(rect);
            }
        }
    }
}

void NumBox::focus(sf::Event::MouseButtonEvent mouse)
{
    sf::Text apple("", *font, absoluteSize.y);
    
    std::vector < int > lengths;
    
    for(int i = 0; i < str.length(); i++)
    {
        apple.setString(str.substr(0, i));
        lengths.push_back(apple.getGlobalBounds().width);
    }
    
    float xpos = mouse.x - absolutePosition.x;
    int item = -1;
    for(int i = 0; i < str.length(); i++)
    {
        if(lengths[i] > xpos)
        {
            item = i;
            break;
        }
    }
    std::cout << "\n\n";
    if(item == -1)
    {
        highlightStart = -1;
        cursorSpot = str.length();
    }
    else
    {
        highlightStart = -1;
        if((xpos - lengths[item-1])/(lengths[item]-lengths[item-1]) > 0.5)
        {
            cursorSpot = item;
        }
        else
        {
            cursorSpot = item-1;
        }
    }
    focused = true;
}

void NumBox::focus()
{
    sf::Text apple("", *font, absoluteSize.y);
    
    std::vector < int > lengths;
    
    for(int i = 0; i < str.length(); i++)
    {
        apple.setString(str.substr(0, i));
        lengths.push_back(apple.getGlobalBounds().width);
    }
    focused = true;
}

void NumBox::unfocus()
{
    focused = false;
}

void NumBox::textEntered(int key)
{
    if(highlightStart != -1)
    {
        if(highlightStart<cursorSpot)
        {
            str.erase(highlightStart, cursorSpot-highlightStart);
            cursorSpot = highlightStart;
        }
        else
        {
            str.erase(cursorSpot, highlightStart-cursorSpot);
        }
        highlightStart = -1;
    }
    if(key == 8)
    {
        // erase
        if(highlightStart == -1)
        {
            if(cursorSpot>0)
            {
                str.erase(cursorSpot-1, 1);
                cursorSpot--;
            }
            else if(cursorSpot == 0)
            {
                str.erase(0, 1);
            }
        }
    }
    else
    {
        std::string output = UnicodeToString(key);
        if(output!="")
        {
            if(str.length()<maxCharacterLength)
            {
                str.insert(cursorSpot, output);
                cursorSpot++;
            }
        }
        else
        {
            // do nothing
        }
    }
}

void NumBox::keyPressed(sf::Keyboard::Key key)
{
    if(key == sf::Keyboard::Left)
    {
        if(leftShift || rightShift)
        {
            // highlight
            if(highlightStart == -1)
            {
                // start highlight
                highlightStart = cursorSpot;
                if(leftCommand || rightCommand)
                {
                    cursorSpot = 0;
                }
                else if(cursorSpot>0)
                {
                    cursorSpot--;
                }
            }
            else
            {
                // continue highlight
                if(leftCommand || rightCommand)
                {
                    cursorSpot = 0;
                }
                else if(cursorSpot>0)
                {
                    cursorSpot--;
                }
            }
        }
        else if(leftCommand || rightCommand)
        {
            cursorSpot = 0;
            highlightStart = -1;
        }
        else
        {
            if(highlightStart == -1)
            {
                if(cursorSpot>0)
                {
                    cursorSpot--;
                }
            }
            else
            {
                if(highlightStart<cursorSpot)
                {
                    cursorSpot = highlightStart;
                }
                highlightStart = -1;
            }
        }
    }
    else if(key == sf::Keyboard::Right)
    {
        if(leftShift || rightShift)
        {
            // highlight
            if(highlightStart == -1)
            {
                // start highlight
                highlightStart = cursorSpot;
                if(leftCommand || rightCommand)
                {
                    cursorSpot = str.length();
                }
                else if(cursorSpot<str.length())
                {
                    cursorSpot++;
                }
            }
            else
            {
                // continue highlight
                if(leftCommand || rightCommand)
                {
                    cursorSpot = str.length();
                }
                else if(cursorSpot<str.length())
                {
                    cursorSpot++;
                }
            }
        }
        else if(leftCommand || rightCommand)
        {
            cursorSpot = str.length();
            highlightStart = -1;
        }
        else
        {
            if(highlightStart == -1)
            {
                if(cursorSpot<str.length())
                {
                    cursorSpot++;
                }
            }
            else
            {
                if(highlightStart>cursorSpot)
                {
                    cursorSpot = highlightStart;
                }
                highlightStart = -1;
            }
        }
    }
    else if(key == sf::Keyboard::LShift)
    {
        leftShift = true;
    }
    else if(key == sf::Keyboard::RShift)
    {
        rightShift = true;
    }
    else if(key == sf::Keyboard::LSystem)
    {
        leftCommand = true;
    }
    else if(key == sf::Keyboard::RSystem)
    {
        rightCommand = true;
    }
    else if(key == sf::Keyboard::A)
    {
        if(leftCommand || rightCommand)
        {
            highlightStart = 0;
            cursorSpot = str.length();
        }
    }
}

void NumBox::keyReleased(int key)
{
    if(key == sf::Keyboard::LShift)
    {
        leftShift = false;
    }
    else if(key == sf::Keyboard::RShift)
    {
        rightShift = false;
    }
    else if(key == sf::Keyboard::LSystem)
    {
        leftCommand = false;
    }
    else if(key == sf::Keyboard::RSystem)
    {
        rightCommand = false;
    }
}

std::string NumBox::UnicodeToString(int key)
{
    if(key == 48)
    {
        return "0";
    }
    else if(key == 49)
    {
        return "1";
    }
    else if(key == 50)
    {
        return "2";
    }
    else if(key == 51)
    {
        return "3";
    }
    else if(key == 52)
    {
        return "4";
    }
    else if(key == 53)
    {
        return "5";
    }
    else if(key == 54)
    {
        return "6";
    }
    else if(key == 55)
    {
        return "7";
    }
    else if(key == 56)
    {
        return "8";
    }
    else if(key == 57)
    {
        return "9";
    }
    else
    {
        return "";
    }
}

std::string NumBox::getValue()
{
    return str;
}

void NumBox::setValue(std::string newString)
{
    str = newString;
}