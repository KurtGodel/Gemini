//
//  PreferencesMenu.cpp
//  Schach
//
//  Created by Thomas Redding on 7/10/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "PreferencesMenu.h"

PreferencesMenu::PreferencesMenu(int* cpuStartTime, int* humanStartTime, SpriteHolder* spriteHolderInput)
{
    cpuTimeStarter = cpuStartTime;
    humanTimeStarter = humanStartTime;
    spriteHolder = spriteHolderInput;
    loadPreferences();
}

void PreferencesMenu::open()
{
    running = true;
    
    loadPreferences();
    
    numBox.clear();
    int hour = (*cpuTimeStarter)/3600000;
    int minute = (*cpuTimeStarter)/60000;
    minute -= 60*hour;
    int second = (*cpuTimeStarter)/1000;
    second -= 3600*hour+60*minute;
    numBox.push_back(NumBox(sf::Vector2f(0.07, 0.12), sf::Vector2f(0.075,0.05)));
    numBox[numBox.size()-1].setValue(std::to_string(hour));
    numBox.push_back(NumBox(sf::Vector2f(0.17, 0.12), sf::Vector2f(0.075,0.05)));
    numBox[numBox.size()-1].setValue(std::to_string(minute));
    numBox.push_back(NumBox(sf::Vector2f(0.27, 0.12), sf::Vector2f(0.075,0.05)));
    numBox[numBox.size()-1].setValue(std::to_string(second));
    
    hour = (*humanTimeStarter)/3600000;
    minute = (*humanTimeStarter)/60000;
    minute -= 60*hour;
    second = (*humanTimeStarter)/1000;
    second -= 3600*hour+60*minute;
    numBox.push_back(NumBox(sf::Vector2f(0.07, 0.24), sf::Vector2f(0.075,0.05)));
    numBox[numBox.size()-1].setValue(std::to_string(hour));
    numBox.push_back(NumBox(sf::Vector2f(0.17, 0.24), sf::Vector2f(0.075,0.05)));
    numBox[numBox.size()-1].setValue(std::to_string(minute));
    numBox.push_back(NumBox(sf::Vector2f(0.27, 0.24), sf::Vector2f(0.075,0.05)));
    numBox[numBox.size()-1].setValue(std::to_string(second));
}

void PreferencesMenu::frame(sf::RenderWindow* window, sf::Vector2f absolutePosition, float absoluteSize, int frameNum)
{
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(absoluteSize, absoluteSize));
    rect.setPosition(absolutePosition);
    rect.setFillColor(sf::Color(0, 0, 0, 192));
    (*window).draw(rect);

    sf::Text text("Preferences", spriteHolder->font, absoluteSize/10);
    text.setColor(sf::Color(255, 0, 0));
    text.setScale(0.5, 0.5);
    text.setPosition(absolutePosition.x+absoluteSize*0.03, absolutePosition.y+absoluteSize*0.03);
    (*window).draw(text);
    
    for(int i=0; i<numBox.size(); i++)
    {
        numBox[i].render(window, sf::Vector2f(absoluteSize, absoluteSize), absolutePosition, frameNum, &spriteHolder->font);
    }
}

void PreferencesMenu::mouseButtonPressed(sf::Event::MouseButtonEvent mouse, float mouseX, float mouseY)
{
    // do stuff
    int i;
    std::cout << "{" << focus << "}";
    for(i=0; i<numBox.size(); i++)
    {
        if(focus == i)
        {
            if(!numBox[i].mouseButtonPressed(mouse))
            {
                focus = -1;
            }
        }
        else
        {
            if(numBox[i].mouseButtonPressed(mouse))
            {
                numBox[focus].unfocus();
                focus = i;
            }
        }
    }
}

void PreferencesMenu::keyDown(sf::Keyboard::Key key)
{
    if(key == sf::Keyboard::Tab)
    {
        if(focus!=-1)
        {
            numBox[focus].unfocus();
        }
        focus++;
        if(focus >= numBox.size())
        {
            focus = -1;
        }
        else
        {
            numBox[focus].focus();
        }
    }
    else if(focus != -1)
    {
        numBox[focus].keyPressed(key);
    }
}

void PreferencesMenu::keyUp(sf::Keyboard::Key key)
{
    if(focus != -1)
    {
        numBox[focus].keyReleased(key);
    }
}

void PreferencesMenu::mouseMove(sf::Event::MouseMoveEvent mouse)
{
    if(focus != -1)
    {
        numBox[focus].mouseMove(mouse);
    }
}

void PreferencesMenu::textEntered(int key)
{
    if(focus != -1)
    {
        numBox[focus].textEntered(key);
    }
}

void PreferencesMenu::loadPreferences()
{
    std::string line;
    std::ifstream myfile (resourcePath()+"_Preferences.txt");
    if (myfile.is_open())
    {
        while(getline(myfile,line))
        {
            loadLine(line);
        }
        myfile.close();
    }
    else
    {
        std::cout << "(Unable to Open _Preferences.txt)\n";
    }
}

void PreferencesMenu::loadLine(std::string str)
{
    int firstTab = -1;
    int lastTab = -1;
    for(int i=0; i<str.size(); i++)
    {
        if(str[i] == '\t')
        {
            if(firstTab == -1)
            {
                firstTab = i;
            }
            lastTab = i;
        }
    }
    if(firstTab <= 0 || lastTab == str.size()-1)
    {
        // format problems
        return;
    }
    
    std::string label = str.substr(0,firstTab);
    std::string value = str.substr(lastTab+1);
    
    if(label == "CpuStartTime")
    {
        *cpuTimeStarter = std::stoi(value);
    }
    else if(label == "HumanStartTime")
    {
        *humanTimeStarter = std::stoi(value);
    }
}

void PreferencesMenu::save()
{
    std::string str = "";
    
    str += "CpuStartTime\t";
    
    int hour = std::stoi(numBox[0].getValue());
    int minute = std::stoi(numBox[1].getValue());
    int second = std::stoi(numBox[2].getValue());
    str += std::to_string((3600*hour+60*minute+second)*1000);
    str += "\n";
    
    str += "HumanStartTime\t";
    
    hour = std::stoi(numBox[3].getValue());
    minute = std::stoi(numBox[4].getValue());
    second = std::stoi(numBox[5].getValue());
    str += std::to_string((3600*hour+60*minute+second)*1000);
    
    
    std::ofstream myfile;
    myfile.open (resourcePath()+"_Preferences.txt");
    myfile << str;
    myfile.close();
}