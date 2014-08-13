//
//  Game.h
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__Game__
#define __Schach__Game__

#include <iostream>
#include "Board.h"
#include "SpriteHolder.h"
#include "PreferencesMenu.h"
#include "PromotionMenu.h"
#include "EndGameMenu.h"
#include "RunAIInput.h"
#include "MoveConverter.h"

class Game
{
public:
    Game(SpriteHolder* spriteHolderPointer, RunAIInput* gameAiCommunicator);
    void frame(sf::RenderWindow* window, sf::Vector2f pos, float size, sf::Vector2f newStretchFactor, bool reduceTime);
    void mouseDown(sf::Event::MouseButtonEvent mouse);
    void mouseUp(sf::Event::MouseButtonEvent mouse);
    void mouseMove(sf::Event::MouseMoveEvent mouse);
    void keyDown(sf::Keyboard::Key key);
    void keyUp(sf::Keyboard::Key key);
    void newGame();
    int cpuStartTime = (60*10+0)*1000;
    int cpuTimeLeft = cpuStartTime;
    int humanStartTime = (60*20+0)*1000;
    int humanTimeLeft = humanStartTime;
private:
    MoveConverter moveConverter;
    void switchSides();
    void runAI();
    void undoMove();
    bool checkForThreeTimeDraw();
    void addToHistory();
    void writeToCommunicator();
    int aiInCheck(int king);
    int humanInCheck(int kingsq);
    void setMoves(int layer);
    bool willMovePutMeInCheck(int king, int from, int to);
    bool attemptHumanMove(const Move& move);
    void makeHumanMove(const Move& move);
    void makeAIMove(const Move& move);
    bool isGameOver(int turn);
    void renderPieces(sf::RenderWindow* window);
    void renderBoard(sf::RenderWindow* window);
    void renderGreen(sf::RenderWindow* window);
    void redoMove();
    void setDuplicate();
    
    PreferencesMenu preferencesMenu;
    PromotionMenu promotionMenu;
    EndGameMenu endGameMenu;
    int historyIndex;
    std::vector < Board > history;
    RunAIInput* gameAiCommunicator;
    sf::Vector2f stretchFactor;
    int frameStartTime;
    int deltaTime = 0;
    int frameNum = 0;
    float squareSize;
    int move[1000];
    int moveLength[2];
    sf::Vector2f adjustedBoardPosition;
    sf::Vector2f adjustedBoardSize;
    sf::Vector2f adjustedSquareSize;
    bool aiWhite = false;
    sf::Vector2f boardPosition;
    float boardSize;
    Board board;
    SpriteHolder* spriteHolder;
    bool isMouseDown = false;
    int selectedSquare = -1;
    int selectingSquare;
    int turn = -1;      // -1 = human's turn, 1 = cpu's turn
    std::vector < unsigned long > duplicate;
};

#endif /* defined(__Schach__Game__) */
