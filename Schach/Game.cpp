//
//  Game.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "Game.h"

Game::Game(SpriteHolder* spriteHolderPointer, RunAIInput* gameAiCommunicator)
: gameAiCommunicator(gameAiCommunicator),
preferencesMenu(&cpuStartTime, &humanStartTime, spriteHolderPointer)
{
    spriteHolder = spriteHolderPointer;
    
    newGame();
    
    stretchFactor.x = 1;
    stretchFactor.y = 1;
    
    while(gameAiCommunicator->lock.try_lock()){}
    gameAiCommunicator->start = false;
    gameAiCommunicator->lock.unlock();
}

void Game::newGame()
{
    while(gameAiCommunicator->lock.try_lock()){}
    gameAiCommunicator->shouldAiBeRunning = false;
    gameAiCommunicator->lock.unlock();
    
    aiWhite = false;
    preferencesMenu.loadPreferences();
    
    // These two lines of code are overwritten by the convertFenToBoard block of code. They serve not as a functional purpose, but as a reminder of where to edit a board's individual squares numerically
    
    char arr[71] = {
        -4, -1, 0,  0,  0,  0,  1,  4,
        -2, -1, 0,  0,  0,  0,  1,  2,
        -3, -1, 0,  0,  0,  0,  1,  3,
        -5, -1, 0,  0,  0,  0,  1,  5,
        -6, -1, 0,  0,  0,  0,  1,  6,
        -3, -1, 0,  0,  0,  0,  1,  3,
        -2, -1, 0,  0,  0,  0,  1,  2,
        -4, -1, 0,  0,  0,  0,  1,  4,
        1,1,1,1,0,0,0};
    
    board.set(arr);
    
    /*
    Board newBoard = moveConverter.convertFenToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", false);
    board.set(newBoard);
    board.castling[0] = 1;      // cpu queen-side
    board.castling[1] = 1;      // cpu king-side
    board.castling[2] = 1;      // human queen-side
    board.castling[3] = 1;      // human king-side
    board.lastMove[0] = 0;
    board.lastMove[0] = 0;
    board.fiftyMoveDrawCounter = 0;
    */
    
    historyIndex = -1;
    addToHistory();
    
    cpuTimeLeft = cpuStartTime;
    humanTimeLeft = humanStartTime;
    turn = -1;
}

void Game::mouseDown(sf::Event::MouseButtonEvent mouse)
{
    isMouseDown = true;
    float mouseX = (mouse.x-boardPosition.x)/boardSize;
    float mouseY= (mouse.y-boardPosition.y)/boardSize;
    
    if(promotionMenu.running)
    {
        int answer = promotionMenu.mouseDown(mouseX, mouseY);
        if(answer == 0)
        {
            // do nothing
        }
        else if(answer > 1 && answer < 6)
        {
            Move m;
            m.from = promotionMenu.from + 100 * answer;
            m.to = promotionMenu.to;
            board.makeMove(m);
            promotionMenu.running = false;
            runAI();
        }
    }
    else if(endGameMenu.running)
    {
        endGameMenu.mouseDown(mouseX, mouseY);
    }
    else
    {
        if(mouseX>1/18.0 && mouseX<=17/18.0 && mouseY>=1/18.0 && mouseY<=17/18.0)
        {
            // mouse clicked inside the board
            int x, y;
            if(aiWhite)
            {
                x = 8.5-9*mouseX;
                y = 9*mouseY-0.5;
            }
            else
            {
                x = 9*mouseX-0.5;
                y = 9*mouseY-0.5;
            }
            if(selectedSquare == -1)
            {
                if(x>=0 && x<=7 && y>=0 && y<=7)
                {
                    selectedSquare = 8*x+(7-y);
                }
            }
            else
            {
                selectingSquare = 8*x+(7-y);
                bool rtn;
                if(turn == -1)
                {
                    Move m;
                    m.from = selectedSquare;
                    m.to = selectingSquare;
                    rtn = attemptHumanMove(m);
                }
                else
                {
                    rtn = false;
                }
                if(rtn)
                {
                    selectingSquare = -1;
                    selectedSquare = -1;
                    if(isGameOver(1))
                    {
                        // game over
                        endGameMenu.open(spriteHolder);
                    }
                    else
                    {
                        runAI();
                    }
                }
                else
                {
                    selectingSquare = -1;
                    selectedSquare = 8*x+(7-y);
                }
            }
        }
    }
}

void Game::mouseUp(sf::Event::MouseButtonEvent mouse)
{
    isMouseDown = false;
}

void Game::mouseMove(sf::Event::MouseMoveEvent mouse)
{
    if(isMouseDown)
    {
        // mouse is being dragged
    }
    else
    {
        // mouse is being moved but not dragged
    }
}

void Game::keyDown(sf::Keyboard::Key key)
{
    if(key==sf::Keyboard::Z)
    {
        undoMove();
    }
    else if(key==sf::Keyboard::Left)
    {
        if(endGameMenu.running)
        {
            endGameMenu.running = false;
        }
        undoMove();
    }
    else if(key==sf::Keyboard::Right)
    {
        redoMove();
    }
    else if(key==sf::Keyboard::S)
    {
        switchSides();
    }
    else if(key==sf::Keyboard::L)
    {
        std::cout << board.toString();
    }
    else if(key==sf::Keyboard::N)
    {
        // make a null move
        if(turn == -1)
        {
            runAI();
        }
        else
        {
            while(!gameAiCommunicator->lock.try_lock()) {}
            gameAiCommunicator->shouldAiBeRunning = false;
            gameAiCommunicator->lock.unlock();
            turn *= -1;
        }
    }
}

void Game::keyUp(sf::Keyboard::Key key)
{
    //
}

void Game::redoMove()
{
    if(turn == -1)
    {
        // human's turn
        if(historyIndex < ((int) history.size())-2)
        {
            historyIndex += 2;
            board.set(history[historyIndex]);
        }
    }
    else
    {
        // cpu's turn
        if(historyIndex<history.size()-1)
        {
            historyIndex++;
            board.set(history[historyIndex]);
        }
    }
}

void Game::undoMove()
{
    if(turn == -1)
    {
        // human's turn
        if(historyIndex>=2)
        {
            historyIndex -= 2;
            board.set(history[historyIndex]);
        }
    }
    else
    {
        // cpu's turn
        if(historyIndex>=1)
        {
            while(!gameAiCommunicator->lock.try_lock()) {}
            gameAiCommunicator->shouldAiBeRunning = false;
            gameAiCommunicator->lock.unlock();
            historyIndex--;
            history.pop_back();
            board.set(history[historyIndex]);
            turn *= -1;
        }
    }
}

void Game::switchSides()
{
    board.switchSides();
    
    aiWhite = !aiWhite;
    if(turn == 1)
    {
        // cpu's turn
        turn *= -1;
        while(!gameAiCommunicator->lock.try_lock()) {}
        gameAiCommunicator->shouldAiBeRunning = false;
        gameAiCommunicator->lock.unlock();
    }
    else
    {
        // human's turn
        runAI();
    }
}

void Game::frame(sf::RenderWindow* window, sf::Vector2f pos, float size, sf::Vector2f newStretchFactor, bool reduceTime)
{
    frameNum++;
    if(frameNum != 1)
    {
        deltaTime = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
        deltaTime -= frameStartTime;
    }
    frameStartTime = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
    
    if(reduceTime)
    {
        if(turn == 1)
        {
            cpuTimeLeft -= deltaTime;
        }
        else
        {
            humanTimeLeft -= deltaTime;
        }
        if(cpuTimeLeft <= 0 || humanTimeLeft <= 0)
        {
            // game over
            while (gameAiCommunicator->lock.try_lock()){}
            gameAiCommunicator->shouldAiBeRunning = false;
            gameAiCommunicator->lock.unlock();
            endGameMenu.open(spriteHolder);
        }
    }
    
    while(!gameAiCommunicator->lock.try_lock()) {}
    if(!gameAiCommunicator->shouldAiBeRunning && turn == 1)
    {
        Move m;
        m.from = gameAiCommunicator->from;
        m.to = gameAiCommunicator->to;
        makeAIMove(m);
        turn *= -1;
    }
    gameAiCommunicator->lock.unlock();
    
    stretchFactor.x = newStretchFactor.x;
    stretchFactor.y = newStretchFactor.y;
    boardPosition.x = pos.x;
    boardPosition.y = pos.y;
    boardSize = size;
    squareSize = boardSize/9;
    
    adjustedBoardPosition.x = pos.x/stretchFactor.x;
    adjustedBoardPosition.y = pos.y/stretchFactor.y;
    adjustedBoardSize.x = boardSize/stretchFactor.x;
    adjustedBoardSize.y = boardSize/stretchFactor.y;
    adjustedSquareSize.x = adjustedBoardSize.x / 9;
    adjustedSquareSize.y = adjustedBoardSize.y / 9;
    
    renderBoard(window);
    if(promotionMenu.running)
    {
        renderPieces(window);
        promotionMenu.render(window, sf::Vector2f(adjustedBoardPosition.x+adjustedSquareSize.x/2, adjustedBoardPosition.y+adjustedSquareSize.y/2), sf::Vector2f(adjustedBoardSize.x*8/9, adjustedBoardSize.y*8/9), aiWhite);
    }
    else if(endGameMenu.running)
    {
        renderPieces(window);
        endGameMenu.render(window, sf::Vector2f(adjustedBoardPosition.x+adjustedSquareSize.x/2, adjustedBoardPosition.y+adjustedSquareSize.y/2), sf::Vector2f(adjustedBoardSize.x*8/9, adjustedBoardSize.y*8/9), aiWhite);
    }
    else
    {
        renderGreen(window);
        renderPieces(window);
    }
    
    renderPrincipleLine(window);
}

bool Game::checkForThreeTimeDraw()
{
    std::vector < uint64_t > hashHistory;
    for(int i=0; i<=historyIndex; i++)
    {
        hashHistory.push_back(history[i].getZorbistHash());
    }
    uint64_t currentHash = board.getZorbistHash();
    
    int counter = 0;
    for(int i=0; i<hashHistory.size(); i++)
    {
        if(currentHash == hashHistory[i])
        {
            counter++;
        }
    }
    if(counter >= 3)
    {
        return true;
    }
    return false;
}

void Game::addToHistory()
{
    if(historyIndex+1 == history.size())
    {
        // pure new move
        history.push_back(board);
        historyIndex++;
    }
    else
    {
        history.erase(history.begin()+historyIndex+1, history.end());
        history.push_back(board);
        historyIndex = history.size()-1;
    }
}

bool Game::attemptHumanMove(const Move& m)
{
    if(m.from == m.to)
    {
        return false;
    }
    setMoves(1);
    for(int i = 500; i < moveLength[1]; i+=2)
    {
        if(m.from == move[i]%100 && m.to == move[i+1])
        {
            if(board.b[move[i]%100] == -1 && move[i+1]%8 == 7)
            {
                makeHumanMove(m);
                return false;
            }
            else
            {
                makeHumanMove(m);
            }
            return true;
        }
    }
    return false;
}

void Game::makeHumanMove(const Move& move)
{
    if(board.b[move.from]==-1 && move.to%8==7)
    {
        promotionMenu.open(spriteHolder, move.from, move.to);
    }
    else {
        board.makeMove(move);
        addToHistory();
    }
}

void Game::makeAIMove(const Move& move)
{
    board.makeMove(move);
    addToHistory();
    return;
    
    board.lastMove[0] = move.from;
    board.lastMove[1] = move.to;
    if(move.from>=100)
    {
        // promotion
        int x = move.from/100;
        if(board.b[move.to]==0)
        {
            board.fiftyMoveDrawCounter++;
        }
        else
        {
            board.fiftyMoveDrawCounter = 0;
        }
        board.b[move.to] = x;
        board.b[move.from%100] = 0;
    }
    else if(board.b[move.from]==6 && (move.to-move.from==16 || move.from-move.to==16))
    {
        // castle
        if(move.to==23)
        {
            board.b[7] = 0;
            board.b[23] = 6;
            board.b[31] = 4;
            board.b[39] = 0;
        }
        else
        {
            board.b[39] = 0;
            board.b[47] = 4;
            board.b[55] = 6;
            board.b[63] = 0;
        }
        board.castling[0] = 0;
        board.castling[1] = 0;
        board.fiftyMoveDrawCounter++;
    }
    else if(board.b[move.from]==1 && board.b[move.to]==0 && (move.to-move.from==7 || move.to-move.from==-9))
    {
        // En passant
        board.b[move.to+1] = 0;
        board.b[move.to] = 1;
        board.b[move.from] = 0;
        board.fiftyMoveDrawCounter = 0;
    }
    else
    {
        // normal move
        if(board.b[move.from]==6)
        {
            board.castling[0] = 0;
            board.castling[1] = 0;
        }
        else if(board.b[move.from]==4)
        {
            if(move.from==7)
            {
                board.castling[0] = 0;
            }
            else if(move.from==63)
            {
                board.castling[1] = 0;
            }
        }
        if(board.b[move.to]==-4)
        {
            if(move.to==0)
            {
                board.castling[2] = 0;
            }
            else
            {
                board.castling[3] = 0;
            }
        }
        
        
        if(board.b[move.to]==0)
        {
            board.fiftyMoveDrawCounter++;
        }
        else
        {
            board.fiftyMoveDrawCounter = 0;
        }
        board.b[move.to] = board.b[move.from];
        board.b[move.from] = 0;
    }
    board.lastMove[0] = move.from%100;
    board.lastMove[1] = move.to;
    addToHistory();
}

bool Game::isGameOver(int turn)
{
    // 1 = cpu
    // -1 = human
    if(board.fiftyMoveDrawCounter == 100)
    {
        return true;
    }
    else if(checkForThreeTimeDraw())
    {
        return true;
    }
    else if(turn==-1)
    {
        // Can human move legally?
        setMoves(1);
        if(moveLength[1]==500)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(turn==1)
    {
        // Can cpu move legally?
        setMoves(0);
        if(moveLength[0]==0)
        {
            setMoves(0);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void Game::setMoves(int layer)
{
    if(layer%2==0)
    {
        int i;
        int j;
        int dist;
        int begin = 500*layer;
        for(i=0;i<64;i++)
        {
            if(board.b[i]>0)
            {
                if(board.b[i]==1)
                {
                    // pawn
                    if(board.b[i-1]==0)
                    {
                        // forward one
                        if(i%8!=1)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin+=2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i-1;
                            begin+=2;
                            move[begin] = 300+i;
                            move[begin+1] = i-1;
                            begin+=2;
                            move[begin] = 400+i;
                            move[begin+1] = i-1;
                            begin+=2;
                            move[begin] = 500+i;
                            move[begin+1] = i-1;
                            begin+=2;
                        }
                        if(i%8==6 && board.b[i-2]==0)
                        {
                            // forward two
                            move[begin] = i;
                            move[begin+1] = i-2;
                            begin += 2;
                        }
                    }
                    if(board.b[i-9]<0 && i/8!=0)
                    {
                        if(i%8!=1)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i-9;
                            begin += 2;
                            move[begin] = 300+i;
                            move[begin+1] = i-9;
                            begin += 2;
                            move[begin] = 400+i;
                            move[begin+1] = i-9;
                            begin += 2;
                            move[begin] = 500+i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    if(board.b[i+7]<0 && i/8!=7)
                    {
                        if(i%8!=1)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i+7;
                            begin += 2;
                            move[begin] = 300+i;
                            move[begin+1] = i+7;
                            begin += 2;
                            move[begin] = 400+i;
                            move[begin+1] = i+7;
                            begin += 2;
                            move[begin] = 500+i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                    }
                    
                    // en passant
                    if(i/8!=7)
                    {
                        if(board.b[i+8]==-1)
                        {
                            if(board.lastMove[1]-board.lastMove[0]==2 &&  board.lastMove[1]==i+8 && board.b[i+7]==0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+7;
                                begin += 2;
                            }
                        }
                    }
                    if(i/8!=0)
                    {
                        if(board.b[i-8]==-1)
                        {
                            if(board.lastMove[1]-board.lastMove[0]==2 &&  board.lastMove[1]==i-8 && board.b[i-9]==0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-9;
                                begin += 2;
                            }
                        }
                    }
                }
                else if(board.b[i]==2)
                {
                    // knight
                    if(i/8==0)
                    {
                        if(i%8==0)
                        {
                            if(board.b[10]<=0)
                            {
                                move[begin] = 0;
                                move[begin+1] = 10;
                                begin += 2;
                            }
                            if(board.b[17]<=0)
                            {
                                move[begin] = 0;
                                move[begin+1] = 17;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[11]<=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 11;
                                begin += 2;
                            }
                            if(board.b[16]<=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 16;
                                begin += 2;
                            }
                            if(board.b[18]<=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 18;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[12]<=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 12;
                                begin += 2;
                            }
                            if(board.b[21]<=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 21;
                                begin += 2;
                            }
                            if(board.b[23]<=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 23;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[13]<=0)
                            {
                                move[begin] = 7;
                                move[begin+1] = 13;
                                begin += 2;
                            }
                            if(board.b[22]<=0)
                            {
                                move[begin] = 7;
                                move[begin+1] = 22;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==1)
                    {
                        if(i%8==0)
                        {
                            if(board.b[2]<=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 2;
                                begin += 2;
                            }
                            if(board.b[18]<=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 18;
                                begin += 2;
                            }
                            if(board.b[25]<=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 25;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[3]<=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 3;
                                begin += 2;
                            }
                            if(board.b[19]<=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 19;
                                begin += 2;
                            }
                            if(board.b[24]<=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 24;
                                begin += 2;
                            }
                            if(board.b[26]<=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 26;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[4]<=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 4;
                                begin += 2;
                            }
                            if(board.b[20]<=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 20;
                                begin += 2;
                            }
                            if(board.b[29]<=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 29;
                                begin += 2;
                            }
                            if(board.b[31]<=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 31;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[5]<=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 5;
                                begin += 2;;
                            }
                            if(board.b[21]<=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 21;
                                begin += 2;;
                            }
                            if(board.b[30]<=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 30;
                                begin += 2;;
                            }
                        }
                        else
                        {
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==6)
                    {
                        if(i%8==0)
                        {
                            if(board.b[33]<=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 33;
                                begin += 2;
                            }
                            if(board.b[42]<=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 42;
                                begin += 2;
                            }
                            if(board.b[58]<=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 58;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[32]<=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 32;
                                begin += 2;
                            }
                            if(board.b[34]<=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 34;
                                begin += 2;
                            }
                            if(board.b[43]<=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 43;
                                begin += 2;
                            }
                            if(board.b[50]<=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 59;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[37]<=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 37;
                                begin += 2;
                            }
                            if(board.b[39]<=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 39;
                                begin += 2;
                            }
                            if(board.b[44]<=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 4;
                                begin += 2;
                            }
                            if(board.b[60]<=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 60;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[38]<=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 38;
                                begin += 2;
                            }
                            if(board.b[45]<=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 45;
                                begin += 2;
                            }
                            if(board.b[61]<=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 61;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==7)
                    {
                        if(i%8==0)
                        {
                            if(board.b[41]<=0)
                            {
                                move[begin] = 56;
                                move[begin+1] = 41;
                                begin += 2;
                            }
                            if(board.b[50]<=0)
                            {
                                move[begin] = 56;
                                move[begin+1] = 50;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[40]<=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 40;
                                begin += 2;
                            }
                            if(board.b[42]<=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 42;
                                begin += 2;
                            }
                            if(board.b[51]<=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 51;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[45]<=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 45;
                                begin += 2;
                            }
                            if(board.b[47]<=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 47;
                                begin += 2;
                            }
                            if(board.b[52]<=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 52;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[46]<=0)
                            {
                                move[begin] = 63;
                                move[begin+1] = 46;
                                begin += 2;
                            }
                            if(board.b[53]<=0)
                            {
                                move[begin] = 63;
                                move[begin+1] = 53;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                        }
                    }
                    else
                    {
                        if(i%8==0)
                        {
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                }
                else if(board.b[i]==3 || board.b[i]==5)
                {
                    // bishop
                    
                    // downward and leftward
                    if(i/8<i%8)
                    {
                        dist = i/8;
                    }
                    else
                    {
                        dist = i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-9*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-9*j;
                        begin += 2;
                        if(board.b[i-9*j]<0)
                        {
                            break;
                        }
                    }
                    
                    // downward and rightward
                    if(7-i/8<i%8)
                    {
                        dist = 7-i/8;
                    }
                    else
                    {
                        dist = i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+7*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+7*j;
                        begin += 2;
                        if(board.b[i+7*j]<0)
                        {
                            break;
                        }
                    }
                    
                    // upward and leftward
                    if(i/8<7-i%8)
                    {
                        dist = i/8;
                    }
                    else
                    {
                        dist = 7-i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-7*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-7*j;
                        begin += 2;
                        if(board.b[i-7*j]<0)
                        {
                            break;
                        }
                    }
                    
                    
                    // upward and rightward
                    if(7-i/8<7-i%8)
                    {
                        dist = 7-i/8;
                    }
                    else
                    {
                        dist = 7-i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+9*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+9*j;
                        begin += 2;
                        if(board.b[i+9*j]<0)
                        {
                            break;
                        }
                    }
                }
                else if(board.b[i]==6)
                {
                    // king
                    if(i==0)
                    {
                        if(board.b[1]<=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 1;
                            begin += 2;
                        }
                        if(board.b[8]<=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 8;
                            begin += 2;
                        }
                        if(board.b[9]<=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 9;
                            begin += 2;
                        }
                    }
                    else if(i==7)
                    {
                        if(board.b[6]<=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 6;
                            begin += 2;
                        }
                        if(board.b[14]<=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 14;
                            begin += 2;
                        }
                        if(board.b[15]<=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 15;
                            begin += 2;
                        }
                    }
                    else if(i==56)
                    {
                        if(board.b[48]<=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 48;
                            begin += 2;
                        }
                        if(board.b[49]<=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 49;
                            begin += 2;
                        }
                        if(board.b[57]<=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 57;
                            begin += 2;
                        }
                    }
                    else if(i==63)
                    {
                        if(board.b[54]<=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 54;
                            begin += 2;
                        }
                        if(board.b[55]<=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 55;
                            begin += 2;
                        }
                        if(board.b[62]<=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 62;
                            begin += 2;
                        }
                    }
                    else if(i/8==0)
                    {
                        if(board.b[i-1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i+8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    else if(i/8==7)
                    {
                        if(board.b[i-1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i-7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i-8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    else if(i%8==0)
                    {
                        if(board.b[i-8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i+1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    else if(i%8==7)
                    {
                        if(board.b[i+8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i-1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i-8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    else
                    {
                        if(board.b[i-9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                        if(board.b[i-8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i-1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i+8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    
                    // castling
                    if(board.castling[0]==1)
                    {
                        // queen-side
                        if(board.b[15]==0 && board.b[23]==0 && board.b[31]==0)
                        {
                            if(aiInCheck(39)==0 && aiInCheck(31)==0 && aiInCheck(23)==0)
                            {
                                move[begin] = 39;
                                move[begin+1] = 23;
                                begin += 2;
                            }
                        }
                    }
                    if(board.castling[1]==1)
                    {
                        // king-side
                        if(board.b[47]==0 && board.b[55]==0)
                        {
                            if(aiInCheck(39)==0 && aiInCheck(47)==0 && aiInCheck(55)==0)
                            {
                                move[begin] = 39;
                                move[begin+1] = 55;
                                begin += 2;
                            }
                        }
                    }
                }
                if(board.b[i]==4 || board.b[i]==5)
                {
                    // rook
                    
                    // downward
                    dist = i%8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-j;
                        begin += 2;
                        if(board.b[i-j]<0)
                        {
                            break;
                        }
                    }
                    
                    // upward
                    dist = 7-i%8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+j;
                        begin += 2;
                        if(board.b[i+j]<0)
                        {
                            break;
                        }
                    }
                    
                    // leftward
                    dist = i/8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-8*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-8*j;
                        begin += 2;
                        if(board.b[i-8*j]<0)
                        {
                            break;
                        }
                    }
                    
                    // rightward
                    dist = 7-i/8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+8*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+8*j;
                        begin += 2;
                        if(board.b[i+8*j]<0)
                        {
                            break;
                        }
                    }
                }
            }
        }
        moveLength[layer] = begin;
        
        
        // -2 = no king, -1 = checkmate (lose), 0 = stalemate, 1 = all normal
        int sto;
        int king = -1;
        for(i=0;i<64;i++)
        {
            if(board.b[i]==6)
            {
                king = i;
                break;
            }
        }
        
        
        for(i=layer*500;i<moveLength[layer];i+=2)
        {
            if(willMovePutMeInCheck(king,move[i],move[i+1]))
            {
                move[i] = 0;
                move[i+1] = 0;
            }
        }
        for(i=layer*500;i<moveLength[layer];i+=2)
        {
            if(move[i]==move[i+1])
            {
                if(move[i]==0)
                {
                    sto = 0;
                    for(j=moveLength[layer]-2;j>i;j-=2)
                    {
                        if(!(move[j]==0 && move[j+1]==0))
                        {
                            move[i] = move[j];
                            move[i+1] =  move[j+1];
                            move[j] = 0;
                            move[j+1] = 0;
                            sto = 1;
                            break;
                        }
                    }
                    if(sto==0)
                    {
                        moveLength[layer] = i;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        int i;
        int j;
        int dist;
        int begin = 500*layer;
        for(i=0;i<64;i++)
        {
            if(board.b[i]<0)
            {
                if(board.b[i]==-1)
                {
                    // pawn
                    if(board.b[i+1]==0)
                    {
                        // forward one
                        if(i%8!=6)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin+=2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i+1;
                            begin+=2;
                            move[begin] = 300+i;
                            move[begin+1] = i+1;
                            begin+=2;
                            move[begin] = 400+i;
                            move[begin+1] = i+1;
                            begin+=2;
                            move[begin] = 500+i;
                            move[begin+1] = i+1;
                            begin+=2;
                        }
                        if(i%8==1 && board.b[i+2]==0)
                        {
                            // forward two
                            move[begin] = i;
                            move[begin+1] = i+2;
                            begin += 2;
                        }
                    }
                    if(board.b[i+9]>0 && i/8!=7)
                    {
                        if(i%8!=6)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i+9;
                            begin += 2;
                            move[begin] = 300+i;
                            move[begin+1] = i+9;
                            begin += 2;
                            move[begin] = 400+i;
                            move[begin+1] = i+9;
                            begin += 2;
                            move[begin] = 500+i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    if(board.b[i-7]>0 && i/8!=0)
                    {
                        if(i%8!=6)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i-7;
                            begin += 2;
                            move[begin] = 300+i;
                            move[begin+1] = i-7;
                            begin += 2;
                            move[begin] = 400+i;
                            move[begin+1] = i-7;
                            begin += 2;
                            move[begin] = 500+i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                    }
                    
                    // en passant
                    if(i/8!=7)
                    {
                        if(board.b[i+8]==1)
                        {
                            if(board.lastMove[0]-board.lastMove[1]==2 &&  board.lastMove[1]==i+8 && board.b[i+9]==0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+9;
                                begin += 2;
                            }
                        }
                    }
                    if(i/8!=0)
                    {
                        if(board.b[i-8]==1)
                        {
                            if(board.lastMove[0]-board.lastMove[1]==2 &&  board.lastMove[1]==i-8 && board.b[i-7]==0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-7;
                                begin += 2;
                            }
                        }
                    }
                }
                else if(board.b[i]==-2)
                {
                    // knight
                    if(i/8==0)
                    {
                        if(i%8==0)
                        {
                            if(board.b[10]>=0)
                            {
                                move[begin] = 0;
                                move[begin+1] = 10;
                                begin += 2;
                            }
                            if(board.b[17]>=0)
                            {
                                move[begin] = 0;
                                move[begin+1] = 17;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[11]>=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 11;
                                begin += 2;
                            }
                            if(board.b[16]>=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 16;
                                begin += 2;
                            }
                            if(board.b[18]>=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 18;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[12]>=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 12;
                                begin += 2;
                            }
                            if(board.b[21]>=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 21;
                                begin += 2;
                            }
                            if(board.b[23]>=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 23;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[13]>=0)
                            {
                                move[begin] = 7;
                                move[begin+1] = 13;
                                begin += 2;
                            }
                            if(board.b[22]>=0)
                            {
                                move[begin] = 7;
                                move[begin+1] = 22;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==1)
                    {
                        if(i%8==0)
                        {
                            if(board.b[2]>=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 2;
                                begin += 2;
                            }
                            if(board.b[18]>=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 18;
                                begin += 2;
                            }
                            if(board.b[25]>=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 25;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[3]>=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 3;
                                begin += 2;
                            }
                            if(board.b[19]>=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 19;
                                begin += 2;
                            }
                            if(board.b[24]>=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 24;
                                begin += 2;
                            }
                            if(board.b[26]>=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 26;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[4]>=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 4;
                                begin += 2;
                            }
                            if(board.b[20]>=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 20;
                                begin += 2;
                            }
                            if(board.b[29]>=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 29;
                                begin += 2;
                            }
                            if(board.b[31]>=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 31;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[5]>=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 5;
                                begin += 2;;
                            }
                            if(board.b[21]>=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 21;
                                begin += 2;;
                            }
                            if(board.b[30]>=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 30;
                                begin += 2;;
                            }
                        }
                        else
                        {
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==6)
                    {
                        if(i%8==0)
                        {
                            if(board.b[33]>=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 33;
                                begin += 2;
                            }
                            if(board.b[42]>=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 42;
                                begin += 2;
                            }
                            if(board.b[58]>=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 58;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[32]>=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 32;
                                begin += 2;
                            }
                            if(board.b[34]>=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 34;
                                begin += 2;
                            }
                            if(board.b[43]>=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 43;
                                begin += 2;
                            }
                            if(board.b[50]>=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 59;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[37]>=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 37;
                                begin += 2;
                            }
                            if(board.b[39]>=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 39;
                                begin += 2;
                            }
                            if(board.b[44]>=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 4;
                                begin += 2;
                            }
                            if(board.b[60]>=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 60;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[38]>=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 38;
                                begin += 2;
                            }
                            if(board.b[45]>=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 45;
                                begin += 2;
                            }
                            if(board.b[61]>=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 61;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==7)
                    {
                        if(i%8==0)
                        {
                            if(board.b[41]>=0)
                            {
                                move[begin] = 56;
                                move[begin+1] = 41;
                                begin += 2;
                            }
                            if(board.b[50]>=0)
                            {
                                move[begin] = 56;
                                move[begin+1] = 50;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[40]>=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 40;
                                begin += 2;
                            }
                            if(board.b[42]>=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 42;
                                begin += 2;
                            }
                            if(board.b[51]>=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 51;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[45]>=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 45;
                                begin += 2;
                            }
                            if(board.b[47]>=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 47;
                                begin += 2;
                            }
                            if(board.b[52]>=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 52;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[46]>=0)
                            {
                                move[begin] = 63;
                                move[begin+1] = 46;
                                begin += 2;
                            }
                            if(board.b[53]>=0)
                            {
                                move[begin] = 63;
                                move[begin+1] = 53;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                        }
                    }
                    else
                    {
                        if(i%8==0)
                        {
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                }
                else if(board.b[i]==-3 || board.b[i]==-5)
                {
                    // bishop
                    
                    // downward and leftward
                    if(i/8<i%8)
                    {
                        dist = i/8;
                    }
                    else
                    {
                        dist = i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-9*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-9*j;
                        begin += 2;
                        if(board.b[i-9*j]>0)
                        {
                            break;
                        }
                    }
                    
                    // downward and rightward
                    if(7-i/8<i%8)
                    {
                        dist = 7-i/8;
                    }
                    else
                    {
                        dist = i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+7*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+7*j;
                        begin += 2;
                        if(board.b[i+7*j]>0)
                        {
                            break;
                        }
                    }
                    
                    // upward and leftward
                    if(i/8<7-i%8)
                    {
                        dist = i/8;
                    }
                    else
                    {
                        dist = 7-i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-7*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-7*j;
                        begin += 2;
                        if(board.b[i-7*j]>0)
                        {
                            break;
                        }
                    }
                    
                    
                    // upward and rightward
                    if(7-i/8<7-i%8)
                    {
                        dist = 7-i/8;
                    }
                    else
                    {
                        dist = 7-i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+9*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+9*j;
                        begin += 2;
                        if(board.b[i+9*j]>0)
                        {
                            break;
                        }
                    }
                }
                else if(board.b[i]==-6)
                {
                    // king
                    if(i==0)
                    {
                        if(board.b[1]>=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 1;
                            begin += 2;
                        }
                        if(board.b[8]>=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 8;
                            begin += 2;
                        }
                        if(board.b[9]>=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 9;
                            begin += 2;
                        }
                    }
                    else if(i==7)
                    {
                        if(board.b[6]>=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 6;
                            begin += 2;
                        }
                        if(board.b[14]>=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 14;
                            begin += 2;
                        }
                        if(board.b[15]>=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 15;
                            begin += 2;
                        }
                    }
                    else if(i==56)
                    {
                        if(board.b[48]>=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 48;
                            begin += 2;
                        }
                        if(board.b[49]>=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 49;
                            begin += 2;
                        }
                        if(board.b[57]>=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 57;
                            begin += 2;
                        }
                    }
                    else if(i==63)
                    {
                        if(board.b[54]>=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 54;
                            begin += 2;
                        }
                        if(board.b[55]>=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 55;
                            begin += 2;
                        }
                        if(board.b[62]>=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 62;
                            begin += 2;
                        }
                    }
                    else if(i/8==0)
                    {
                        if(board.b[i-1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i+8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    else if(i/8==7)
                    {
                        if(board.b[i-1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i-7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i-8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    else if(i%8==0)
                    {
                        if(board.b[i-8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i+1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    else if(i%8==7)
                    {
                        if(board.b[i+8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i-1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i-8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    else
                    {
                        if(board.b[i-9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                        if(board.b[i-8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i-1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i+8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    
                    // castling
                    if(board.castling[2]==1)
                    {
                        // queen-side
                        if(board.b[8]==0 && board.b[16]==0 && board.b[24]==0)
                        {
                            if(humanInCheck(32)==0 && humanInCheck(24)==0 && humanInCheck(16)==0)
                            {
                                move[begin] = 32;
                                move[begin+1] = 16;
                                begin += 2;
                            }
                        }
                    }
                    if(board.castling[3]==1)
                    {
                        // king-side
                        if(board.b[40]==0 && board.b[48]==0)
                        {
                            if(humanInCheck(32)==0 && humanInCheck(40)==0 && humanInCheck(48)==0)
                            {
                                move[begin] = 32;
                                move[begin+1] = 48;
                                begin += 2;
                            }
                        }
                    }
                }
                if(board.b[i]==-4 || board.b[i]==-5)
                {
                    // rook
                    
                    // downward
                    dist = i%8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-j;
                        begin += 2;
                        if(board.b[i-j]>0)
                        {
                            break;
                        }
                    }
                    
                    // upward
                    dist = 7-i%8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+j;
                        begin += 2;
                        if(board.b[i+j]>0)
                        {
                            break;
                        }
                    }
                    
                    // leftward
                    dist = i/8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-8*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-8*j;
                        begin += 2;
                        if(board.b[i-8*j]>0)
                        {
                            break;
                        }
                    }
                    
                    // rightward
                    dist = 7-i/8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+8*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+8*j;
                        begin += 2;
                        if(board.b[i+8*j]>0)
                        {
                            break;
                        }
                    }
                }
            }
        }
        moveLength[layer] = begin;
        
        // -2 = no king, -1 = checkmate (lose), 0 = stalemate, 1 = normal
        int sto;
        int king = -1;
        for(i=0;i<64;i++)
        {
            if(board.b[i]==-6)
            {
                king = i;
                break;
            }
        }
        for(i=layer*500;i<moveLength[layer];i+=2)
        {
            if(willMovePutMeInCheck(king,move[i],move[i+1]))
            {
                move[i] = 0;
                move[i+1] = 0;
            }
        }
        for(i=layer*500;i<moveLength[layer];i+=2)
        {
            if(move[i]==move[i+1])
            {
                if(move[i]==0)
                {
                    sto = 0;
                    for(j=moveLength[layer]-2;j>i;j-=2)
                    {
                        if(!(move[j]==0 && move[j+1]==0))
                        {
                            move[i] = move[j];
                            move[i+1] =  move[j+1];
                            move[j] = 0;
                            move[j+1] = 0;
                            sto = 1;
                            break;
                        }
                    }
                    if(sto==0)
                    {
                        moveLength[layer] = i;
                        break;
                    }
                }
            }
        }
    }
}

int Game::aiInCheck(int king)
{
    // 1 = in check; 0 = safe
    int i;
    // pawn attacks
    if(king%8!=0)
    {
        if(king/8!=0)
        {
            if(board.b[king-9]==-1)
            {
                return 1;
            }
        }
        if(king/8!=7)
        {
            if(board.b[king+7]==-1)
            {
                return 1;
            }
        }
    }
    
    // knight attacks
    if(king%8>0)
    {
        if(king/8>1 && board.b[king-17]==-2)
        {
            return 1;
        }
        if(king/8<6 && board.b[king+15]==-2)
        {
            return 1;
        }
    }
    if(king%8>1)
    {
        if(king/8>0 && board.b[king-10]==-2)
        {
            return 1;
        }
        if(king/8<7 && board.b[king+6]==-2)
        {
            return 1;
        }
    }
    if(king%8<6)
    {
        if(king/8>0 && board.b[king-6]==-2)
        {
            return 1;
        }
        if(king/8<7 && board.b[king+10]==-2)
        {
            return 1;
        }
    }
    if(king%8<7)
    {
        if(king/8>1 && board.b[king-15]==-2)
        {
            return 1;
        }
        if(king/8<6 && board.b[king+17]==-2)
        {
            return 1;
        }
    }
    
    // bishop attacks
    int dist;
    // down-left
    if(king/8<king%8)
    {
        dist = king/8;
    }
    else
    {
        dist = king%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[king-9*i]==-3 || board.b[king-9*i]==-5)
        {
            return 1;
        }
        if(board.b[king-9*i]!=0)
        {
            break;
        }
    }
    // up-right
    if(king/8<king%8)
    {
        dist = 7-king%8;
    }
    else
    {
        dist = 7-king/8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[king+9*i]==-3 || board.b[king+9*i]==-5)
        {
            return 1;
        }
        if(board.b[king+9*i]!=0)
        {
            break;
        }
    }
    // down-right
    if(7-king/8<king%8)
    {
        dist = 7-king/8;
    }
    else
    {
        dist = king%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[king+7*i]==-3 || board.b[king+7*i]==-5)
        {
            return 1;
        }
        if(board.b[king+7*i]!=0)
        {
            break;
        }
    }
    // down-left
    if(king/8<7-king%8)
    {
        dist = king/8;
    }
    else
    {
        dist = 7-king%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[king-7*i]==-3 || board.b[king-7*i]==-5)
        {
            return 1;
        }
        if(board.b[king-7*i]!=0)
        {
            break;
        }
    }
    
    // rook attacks
    // forward
    dist = 7-king%8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[king+i]==-4 || board.b[king+i]==-5)
        {
            return 1;
        }
        if(board.b[king+i]!=0)
        {
            break;
        }
    }
    
    // backward
    dist = king%8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[king-i]==-4 || board.b[king-i]==-5)
        {
            return 1;
        }
        if(board.b[king-i]!=0)
        {
            break;
        }
    }
    // left
    dist = king/8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[king-8*i]==-4 || board.b[king-8*i]==-5)
        {
            return 1;
        }
        if(board.b[king-8*i]!=0)
        {
            break;
        }
    }
    
    // right
    dist = 7-king/8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[king+8*i]==-4 || board.b[king+8*i]==-5)
        {
            return 1;
        }
        if(board.b[king+8*i]!=0)
        {
            break;
        }
    }
    
    // king attacks
    if(king/8>0)
    {
        if(board.b[king-8]==-6)
        {
            return 1;
        }
        if(king%8>0 && board.b[king-9]==-6)
        {
            return 1;
        }
        if(king%8<7 && board.b[king-7]==-6)
        {
            return 1;
        }
    }
    if(king/8<7)
    {
        if(board.b[king+8]==-6)
        {
            return 1;
        }
        if(king%8>0 && board.b[king+7]==-6)
        {
            return 1;
        }
        if(king%8<7 && board.b[king+9]==-6)
        {
            return 1;
        }
    }
    if(king%8>0 && board.b[king-1]==-6)
    {
        return 1;
    }
    if(king%8<7 && board.b[king+1]==-6)
    {
        return 1;
    }
    return 0;
}

int Game::humanInCheck(int kingsq)
{
    // 1 = in check; 0 = safe; -1 = no king
    int i;
    // pawn attacks
    if(kingsq%8!=7)
    {
        if(kingsq/8!=0)
        {
            if(board.b[kingsq-7]==1)
            {
                return 1;
            }
        }
        if(kingsq/8!=7)
        {
            if(board.b[kingsq+9]==1)
            {
                return 1;
            }
        }
    }
    
    // knight attacks
    if(kingsq%8>0)
    {
        if(kingsq/8>1 && board.b[kingsq-17]==2)
        {
            return 1;
        }
        if(kingsq/8<6 && board.b[kingsq+15]==2)
        {
            return 1;
        }
    }
    if(kingsq%8>1)
    {
        if(kingsq/8>0 && board.b[kingsq-10]==2)
        {
            return 1;
        }
        if(kingsq/8<7 && board.b[kingsq+6]==2)
        {
            return 1;
        }
    }
    if(kingsq%8<6)
    {
        if(kingsq/8>0 && board.b[kingsq-6]==2)
        {
            return 1;
        }
        if(kingsq/8<7 && board.b[kingsq+10]==2)
        {
            return 1;
        }
    }
    if(kingsq%8<7)
    {
        if(kingsq/8>1 && board.b[kingsq-15]==2)
        {
            return 1;
        }
        if(kingsq/8<6 && board.b[kingsq+17]==2)
        {
            return 1;
        }
    }
    // bishop attacks
    int dist;
    // down-left
    if(kingsq/8<kingsq%8)
    {
        dist = kingsq/8;
    }
    else
    {
        dist = kingsq%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq-9*i]==3 || board.b[kingsq-9*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq-9*i]!=0)
        {
            break;
        }
    }
    // up-right
    if(kingsq/8<kingsq%8)
    {
        dist = 7-kingsq%8;
    }
    else
    {
        dist = 7-kingsq/8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq+9*i]==3 || board.b[kingsq+9*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq+9*i]!=0)
        {
            break;
        }
    }
    // down-right
    if(7-kingsq/8<kingsq%8)
    {
        dist = 7-kingsq/8;
    }
    else
    {
        dist = kingsq%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq+7*i]==3 || board.b[kingsq+7*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq+7*i]!=0)
        {
            break;
        }
    }
    // up-left
    if(kingsq/8<7-kingsq%8)
    {
        dist = kingsq/8;
    }
    else
    {
        dist = 7-kingsq%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq-7*i]==3 || board.b[kingsq-7*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq-7*i]!=0)
        {
            break;
        }
    }
    
    // rook attacks
    // forward
    dist = 7-kingsq%8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq+i]==4 || board.b[kingsq+i]==5)
        {
            return 1;
        }
        if(board.b[kingsq+i]!=0)
        {
            break;
        }
    }
    
    // backward
    dist = kingsq%8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq-i]==4 || board.b[kingsq-i]==5)
        {
            return 1;
        }
        if(board.b[kingsq-i]!=0)
        {
            break;
        }
    }
    
    // left
    dist = kingsq/8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq-8*i]==4 || board.b[kingsq-8*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq-8*i]!=0)
        {
            break;
        }
    }
    
    // right
    dist = 7-kingsq/8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq+8*i]==4 || board.b[kingsq+8*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq+8*i]!=0)
        {
            break;
        }
    }
    
    // kingsq attacks
    if(kingsq/8>0)
    {
        if(board.b[kingsq-8]==6)
        {
            return 1;
        }
        if(kingsq%8>0 && board.b[kingsq-9]==6)
        {
            return 1;
        }
        if(kingsq%8<7 && board.b[kingsq-7]==6)
        {
            return 1;
        }
    }
    if(kingsq/8<7)
    {
        if(board.b[kingsq+8]==6)
        {
            return 1;
        }
        if(kingsq%8>0 && board.b[kingsq+7]==6)
        {
            return 1;
        }
        if(kingsq%8<7 && board.b[kingsq+9]==6)
        {
            return 1;
        }
    }
    if(kingsq%8>0 && board.b[kingsq-1]==6)
    {
        return 1;
    }
    if(kingsq%8<7 && board.b[kingsq+1]==6)
    {
        return 1;
    }
    return 0;
}

bool Game::willMovePutMeInCheck(int king, int from, int to)
{
    int sto, stoB;
    int answer;
    if(board.b[king]==6)
    {
        // cpu
        if(from>=100)
        {
            // promotion
            sto = board.b[to];
            board.b[to] = (int) (from/100);
            board.b[from%100] = 0;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                stoB = board.fiftyMoveDrawCounter;
                board.fiftyMoveDrawCounter = 0;
            }
            
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[to]==-4)
            {
                if(to==0)
                {
                    board.castling[2] = 0;
                }
                else if(to==56)
                {
                    board.castling[3] = 0;
                }
            }
            
            answer = aiInCheck(king);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            
            if(sto==0)
            {
                board.fiftyMoveDrawCounter--;
            }
            else
            {
                board.fiftyMoveDrawCounter = stoB;
            }
            board.fiftyMoveDrawCounter--;
            board.b[to] = sto;
            board.b[from%100] = 1;
        }
        else if(board.b[from]==6 && (to-from==16 || from-to==16))
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            board.castling[0] = 0;
            board.castling[1] = 0;
            board.fiftyMoveDrawCounter++;
            // castle
            if(to==23)
            {
                board.b[7] = 0;
                board.b[23] = 6;
                board.b[31] = 4;
                board.b[39] = 0;
            }
            else
            {
                board.b[39] = 0;
                board.b[47] = 4;
                board.b[55] = 6;
                board.b[63] = 0;
            }
            
            answer = aiInCheck(to);
            
            if(to==23)
            {
                board.b[7] = 4;
                board.b[23] = 0;
                board.b[31] = 0;
                board.b[39] = 6;
            }
            else
            {
                board.b[39] = 6;
                board.b[47] = 0;
                board.b[55] = 0;
                board.b[63] = 4;
            }
            board.fiftyMoveDrawCounter--;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
        }
        else if(board.b[from]==1 && board.b[to]==0 && (to-from==7 || to-from==-9))
        {
            // En passant
            board.b[to+1] = 0;
            board.b[to] = 1;
            board.b[from] = 0;
            stoB = board.fiftyMoveDrawCounter;
            board.fiftyMoveDrawCounter = 0;
            
            answer = aiInCheck(king);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[to+1] = -1;
            board.b[to] = 0;
            board.b[from] = 1;
        }
        else
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[from]==6)
            {
                board.castling[0] = 0;
                board.castling[1] = 0;
            }
            else if(board.b[from]==4)
            {
                if(from==7)
                {
                    board.castling[0] = 0;
                }
                else if(from==63)
                {
                    board.castling[1] = 0;
                }
            }
            if(board.b[to]==-4)
            {
                if(to==0)
                {
                    board.castling[2] = 0;
                }
                else if(to==56)
                {
                    board.castling[3] = 0;
                }
            }
            
            
            sto = board.b[to];
            board.b[to] = board.b[from];
            board.b[from] = 0;
            
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                stoB = board.fiftyMoveDrawCounter;
                board.fiftyMoveDrawCounter = 0;
            }
            
            if(board.b[to]==6)
            {
                answer = aiInCheck(to);
            }
            else
            {
                answer = aiInCheck(king);
            }
            
            if(sto==0)
            {
                board.fiftyMoveDrawCounter--;
            }
            else
            {
                board.fiftyMoveDrawCounter = stoB;
            }
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            board.b[from] = board.b[to];
            board.b[to] = sto;
        }
    }
    else
    {
        // human
        if(from>=100)
        {
            // promotion
            sto = board.b[to];
            board.b[to] = (int) (from/100);
            board.b[from%100] = 0;
            board.fiftyMoveDrawCounter++;
            
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[to]==4)
            {
                if(to==7)
                {
                    board.castling[0] = 0;
                }
                else
                {
                    board.castling[1] = 0;
                }
            }
            
            answer = humanInCheck(king);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            
            board.fiftyMoveDrawCounter--;
            board.b[to] = sto;
            board.b[from%100] = -1;
        }
        else if(board.b[from]==-6 && (to-from==16 || from-to==16))
        {
            int sto0 = board.castling[2];
            int sto1 = board.castling[3];
            board.castling[2] = 0;
            board.castling[3] = 0;
            // castling
            if(to==16)
            {
                board.b[0] = 0;
                board.b[16] = -6;
                board.b[24] = -4;
                board.b[32] = 0;
            }
            else
            {
                board.b[32] = 0;
                board.b[40] = -4;
                board.b[48] = -6;
                board.b[56] = 0;
            }
            board.fiftyMoveDrawCounter++;
            
            answer = humanInCheck(to);
            
            board.fiftyMoveDrawCounter--;
            if(to==16)
            {
                board.b[0] = -4;
                board.b[16] = 0;
                board.b[24] = 0;
                board.b[32] = -6;
            }
            else
            {
                board.b[32] = -6;
                board.b[40] = 0;
                board.b[48] = 0;
                board.b[56] = -4;
            }
            board.castling[2] = sto0;
            board.castling[3] = sto1;
        }
        else if(board.b[from]==-1 && board.b[to]==0 && (to-from==9 || to-from==-7))
        {
            // En passant
            board.b[to-1] = 0;
            board.b[to] = -1;
            board.b[from] = 0;
            stoB = board.fiftyMoveDrawCounter;
            board.fiftyMoveDrawCounter = 0;
            
            answer = humanInCheck(king);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[to-1] = 1;
            board.b[to] = 0;
            board.b[from] = -1;
        }
        else
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[from]==-6)
            {
                board.castling[2] = 0;
                board.castling[3] = 0;
            }
            else if(board.b[from]==-4)
            {
                if(from==0)
                {
                    board.castling[2] = 0;
                }
                else if(from==56)
                {
                    board.castling[3] = 0;
                }
            }
            if(board.b[to]==4)
            {
                if(to==7)
                {
                    board.castling[0] = 0;
                }
                else
                {
                    board.castling[1] = 0;
                }
            }
            
            sto = board.b[to];
            board.b[to] = board.b[from];
            board.b[from] = 0;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                stoB = board.fiftyMoveDrawCounter;
                board.fiftyMoveDrawCounter = 0;
            }
            
            if(board.b[to]==-6)
            {
                answer = humanInCheck(to);
            }
            else
            {
                answer = humanInCheck(king);
            }
            
            if(sto==0)
            {
                board.fiftyMoveDrawCounter--;
            }
            else
            {
                board.fiftyMoveDrawCounter = stoB;
            }
            board.b[from] = board.b[to];
            board.b[to] = sto;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
        }
    }
    return answer;
}

void Game::writeToCommunicator()
{
    while(!gameAiCommunicator->lock.try_lock()) {}
    gameAiCommunicator->board.set(board);
    
    
    gameAiCommunicator->duplicate.resize(duplicate.size());
    for(int i = 0; i < duplicate.size(); i++) {
        gameAiCommunicator->duplicate[i] = duplicate[i];
    }
    gameAiCommunicator->cpuTimeLeft = cpuTimeLeft;
    gameAiCommunicator->isAIBlack = !aiWhite;
    
    gameAiCommunicator->useOpeningBook = true;              // keep true until future notice
    gameAiCommunicator->useLateMoveReductions = true;       // keep true until future notice
    gameAiCommunicator->useNullMove = true;                 // keep true until future notice
    
    gameAiCommunicator->start = true;
    gameAiCommunicator->shouldAiBeRunning = true;
    gameAiCommunicator->lock.unlock();
}

void Game::setDuplicate()
{
    std::vector < uint64_t > hashHistory;
    for(int i=0; i<=historyIndex; i++)
    {
        hashHistory.push_back(history[i].getZorbistHash());
    }
    duplicate.clear();
    for(int i=0; i<hashHistory.size(); i++)
    {
        for(int j=i+1; j<hashHistory.size(); j++)
        {
            if(hashHistory[i] == hashHistory[j])
            {
                duplicate.push_back(hashHistory[i]);
                break;
            }
        }
    }
}

void Game::runAI()
{
    setDuplicate();
    writeToCommunicator();
    turn *= -1;
}

void Game::renderBoard(sf::RenderWindow* window)
{
    sf::RectangleShape rect;
    rect.setPosition(adjustedBoardPosition.x, adjustedBoardPosition.y);
    rect.setSize(sf::Vector2f(adjustedBoardSize.x, adjustedBoardSize.y));
    rect.setFillColor(sf::Color(199, 121, 52));
    rect.setFillColor(sf::Color(255, 196, 138));
    rect.setFillColor(sf::Color((255+199)/2, (196+121)/2, (138+52)/2));
    window->draw(rect);
    
    spriteHolder->boardSprite.setPosition(adjustedBoardPosition.x+adjustedSquareSize.x/2, adjustedBoardPosition.y+adjustedSquareSize.y/2);
    spriteHolder->boardSprite.setScale(adjustedBoardSize.x/1125, adjustedBoardSize.y/1125);
    window->draw(spriteHolder->boardSprite);
    
    sf::Text text("", spriteHolder->font, 100);
    text.setScale(adjustedSquareSize.x/200, adjustedSquareSize.y/200);
    text.setColor(sf::Color(0, 0, 0));
    
    int num;
    for(int i = 0; i < 8 ; i++)
    {
        if(aiWhite)
        {
            num = i+1;
        }
        else
        {
            num = 8-i;
        }
        if(num == 1)
        {
            text.setPosition((0.17)*adjustedSquareSize.x+adjustedBoardPosition.x, (i+0.85)*adjustedSquareSize.y+adjustedBoardPosition.y);
        }
        else
        {
            text.setPosition((0.10)*adjustedSquareSize.x+adjustedBoardPosition.x, (i+0.85)*adjustedSquareSize.y+adjustedBoardPosition.y);
        }
        text.setString(std::to_string(num));
        (*window).draw(text);
    }
    for(int i = 0; i < 8 ; i++)
    {
        if(aiWhite)
        {
            num = i+1;
        }
        else
        {
            num = 8-i;
        }
        if(num == 1)
        {
            text.setPosition((8.65)*adjustedSquareSize.x+adjustedBoardPosition.x, (i+0.75)*adjustedSquareSize.y+adjustedBoardPosition.y);
        }
        else
        {
            text.setPosition((8.60)*adjustedSquareSize.x+adjustedBoardPosition.x, (i+0.75)*adjustedSquareSize.y+adjustedBoardPosition.y);
        }
        text.setString(std::to_string(num));
        (*window).draw(text);
    }
    
    int i = 0;
    if(aiWhite)
    {
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("h");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("g");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("f");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("e");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("d");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("c");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("b");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("a");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
    }
    else
    {
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("a");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("b");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("c");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("d");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("e");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("f");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("g");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
        i++;
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (-0.10)*adjustedSquareSize.y+adjustedBoardPosition.y);
        text.setString("h");
        (*window).draw(text);
        text.setPosition((0.85+i)*adjustedSquareSize.x+adjustedBoardPosition.x, (8.38)*adjustedSquareSize.y+adjustedBoardPosition.y);
        (*window).draw(text);
    }
}

void Game::renderGreen(sf::RenderWindow* window)
{
    if(selectedSquare != -1)
    {
        int x,y;
        if(aiWhite)
        {
            x = 7-selectedSquare/8;
            y = 7-selectedSquare%8;
        }
        else
        {
            x = selectedSquare/8;
            y = 7-selectedSquare%8;
        }
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(adjustedSquareSize.x, adjustedSquareSize.y));
        rect.setPosition(100*(x+0.5)*adjustedSquareSize.x/100+adjustedBoardPosition.x, 100*(y+0.5)*adjustedSquareSize.y/100+adjustedBoardPosition.y);
        rect.setFillColor(sf::Color(0, 255, 0, 128));
        window->draw(rect);
        
        if(board.b[selectedSquare] < 0)
        {
            setMoves(1);
            std::vector < int > legalMoves;
            for(int i=500; i<moveLength[1]; i+=2)
            {
                if(move[i]%100 == selectedSquare)
                {
                    legalMoves.push_back(move[i+1]);
                }
            }
            rect.setFillColor(sf::Color(0, 255, 0, 128));
            for(int i=0; i<legalMoves.size(); i++)
            {
                if(aiWhite)
                {
                    x = 7-legalMoves[i]/8;
                    y = 7-legalMoves[i]%8;
                }
                else
                {
                    x = legalMoves[i]/8;
                    y = 7-legalMoves[i]%8;
                }
                rect.setPosition(100*(x+0.5)*adjustedSquareSize.x/100+adjustedBoardPosition.x, 100*(y+0.5)*adjustedSquareSize.y/100+adjustedBoardPosition.y);
                window->draw(rect);
            }
        }
    }
}

void Game::renderPieces(sf::RenderWindow* window)
{
    int pieceNum;
    float AX = 0;
    float AY = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(aiWhite)
            {
                if(board.b[8*(7-i)+j]!=0)
                {
                    pieceNum = board.b[8*(7-i)+j]+6;
                    (*spriteHolder).piece[pieceNum].sprite.setScale(adjustedSquareSize.x/300, adjustedSquareSize.y/300);
                    (*spriteHolder).piece[pieceNum].sprite.setPosition((100*(i+0.5)+(*spriteHolder).piece[pieceNum].adjustX+AX)*adjustedSquareSize.x/100+adjustedBoardPosition.x,(100*(7.5-j)+(*spriteHolder).piece[pieceNum].adjustY+AY)*adjustedSquareSize.y/100+adjustedBoardPosition.y);
                    (*window).draw((*spriteHolder).piece[pieceNum].sprite);
                }
            }
            else
            {
                if(board.b[8*i+j]!=0)
                {
                    pieceNum = 6-board.b[8*i+j];
                    (*spriteHolder).piece[pieceNum].sprite.setScale(adjustedSquareSize.x/300, adjustedSquareSize.y/300);
                    (*spriteHolder).piece[pieceNum].sprite.setPosition((100*(i+0.5)+(*spriteHolder).piece[pieceNum].adjustX+AX)*adjustedSquareSize.x/100+adjustedBoardPosition.x,(100*(7.5-j)+(*spriteHolder).piece[pieceNum].adjustY+AY)*adjustedSquareSize.y/100+adjustedBoardPosition.y);
                    (*window).draw((*spriteHolder).piece[pieceNum].sprite);
                }
            }
        }
    }
}

void Game::renderPrincipleLine(sf::RenderWindow* window)
{
    std::vector < std::string > principleLine;
    bool principleLineBasedOnOpeningBook;
    while(gameAiCommunicator->lock.try_lock());
    for(int i=0; i<gameAiCommunicator->principleLine.size(); i++)
    {
        principleLine.push_back(gameAiCommunicator->principleLine[i]);
    }
    principleLineBasedOnOpeningBook = gameAiCommunicator->principleLineBasedOnOpeningBook;
    gameAiCommunicator->lock.unlock();
    
    if(principleLineBasedOnOpeningBook)
    {
        std::string str = "";
        for(int i=0; i<principleLine.size(); i++)
        {
            str += principleLine[i] + "\n";
        }
        sf::Text text(str, spriteHolder->font, 20);
        text.setScale(1.0, 1.0);
        text.setColor(sf::Color(0, 255, 0));
        text.setPosition(600, 100);
        window->draw(text);
    }
    else
    {
        std::string str = "";
        int maxNumberOfMoves = 4;   // moves - not ply!
        
        if(aiWhite)
        {
            str += "1. ";
        }
        else
        {
            for(int i=0; i<maxNumberOfMoves; i++)
            {
                if(2*i < principleLine.size())
                {
                    str += principleLine[2*i];
                    str += " ";
                }
                else
                {
                    break;
                }
                
                
                if(2*i+1 < principleLine.size())
                {
                    str += "\n";
                    str += std::to_string(i+2);
                    str += ". ";
                    str += principleLine[2*i+1];
                    str += " ";
                }
                else
                {
                    break;
                }
            }
        }
        
        sf::Text text(str, spriteHolder->font, 20);
        text.setScale(1.0, 1.0);
        text.setColor(sf::Color(0, 255, 0));
        text.setPosition(600, 100);
        window->draw(text);
    }
}