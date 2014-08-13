//
//  AI.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "AI.h"

AI::AI(RunAIInput* gameAiCommunicator)
:evaluator(&board), gameAiCommunicator(gameAiCommunicator)
{
    char arr[] = {
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0
    };
    board.set(arr);
}

extern int numberOfEvals;

std::vector < std::string > AI::getPrincipleLine(int from, int to, bool isAIBlack)
{
    std::vector < std::string > rtn;
    if(isAIBlack)
    {
        rtn.push_back(moveConverter.convertMoveToString(&board, from, to, 1));
    }
    else
    {
        rtn.push_back(moveConverter.convertMoveToString(&board, from, to, 0));
    }
    
    getPrincipleLineMakeMove(from, to, 1, &rtn, isAIBlack);
    
    return rtn;
}

void AI::getPrincipleLineTree(int layer, std::vector<std::string>* rtn, bool isAIBlack)
{
    if(layer > 10)
    {
        return;
    }
    Node* node = searchTranspositionTable();
    if(node)
    {
        setMoves(layer);
        if(node->bestMove != -1)
        {
            int from = node->bestMove/100;
            int to = node->bestMove%100;
            if(isAIBlack)
            {
                rtn->push_back(moveConverter.convertMoveToString(&board, from, to, 1));
            }
            else
            {
                rtn->push_back(moveConverter.convertMoveToString(&board, from, to, 0));
            }
            getPrincipleLineMakeMove(from, to, layer+1, rtn, isAIBlack);
        }
    }
}

void AI::getPrincipleLineMakeMove(int from, int to, int layer, std::vector<std::string>* rtn, bool isAIBlack)
{
    int sto, stoB;
    if(layer%2==1)
    {
        // cpu
        if(from>=100)
        {
            // promotion
            
            sto = board.b[to];
            board.b[to] = (int) (from/100);
            board.b[from%100] = 0;
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
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
            
            getPrincipleLineTree(layer, rtn, isAIBlack);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            board.fiftyMoveDrawCounter = stoB;
            board.b[to] = sto;
            board.b[from%100] = 1;
        }
        else if(board.b[from]==6 && (to-from==16 || from-to==16))
        {
            
            // castle
            
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            board.castling[0] = 0;
            board.castling[1] = 0;
            board.fiftyMoveDrawCounter++;
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
            
            getPrincipleLineTree(layer, rtn, isAIBlack);
            
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
            
            getPrincipleLineTree(layer, rtn, isAIBlack);
            
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
            
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
            getPrincipleLineTree(layer, rtn, isAIBlack);
            
            board.fiftyMoveDrawCounter = stoB;
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
            board.b[to] *= -1;
            board.b[from%100] = 0;
            stoB = board.fiftyMoveDrawCounter;
            
            if(sto == 0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
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
            
            getPrincipleLineTree(layer, rtn, isAIBlack);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[to] = sto;
            board.b[from%100] = -1;
        }
        else if(board.b[from]==-6 && (to-from==16 || from-to==16))
        {
            // castling
            
            
            int sto0 = board.castling[2];
            int sto1 = board.castling[3];
            board.castling[2] = 0;
            board.castling[3] = 0;
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
            
            getPrincipleLineTree(layer, rtn, isAIBlack);
            
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
            
            getPrincipleLineTree(layer, rtn, isAIBlack);
            
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
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
            getPrincipleLineTree(layer, rtn, isAIBlack);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[from] = board.b[to];
            board.b[to] = sto;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
        }
    }
}

void AI::addHashToTranspositionTable(uint64_t hash, int value, int bestMove, int betaCutoff, float depthSearched)
{
    while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
    gameAiCommunicator->transpositionTable->insert(hash, bestMove, depthSearched);
    gameAiCommunicator->transpositionTable->lock.unlock();
}

Node* AI::searchTranspositionTableForHash(uint64_t hash)
{
    while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
    Node* node = gameAiCommunicator->transpositionTable->searchFor(hash);
    gameAiCommunicator->transpositionTable->lock.unlock();
    return node;
}

void AI::run()
{
    while(!gameAiCommunicator->lock.try_lock()) {}
    if(gameAiCommunicator->shouldAiBeRunning)
    {
        // run ai
        
        bool changeInBoard = !board.set(gameAiCommunicator->board);
        
        if(!changeInBoard)
        {
            changeInBoard = gameAiCommunicator->start;
        }
        gameAiCommunicator->start = false;
        cpuTimeLeft = gameAiCommunicator->cpuTimeLeft;
        gameAiCommunicator->lock.unlock();
        
        setMoves(0);    // get legal moves
        
        if(moveLength[0] == 0)
        {
            // no legal move
            while(!gameAiCommunicator->lock.try_lock()) {}
            gameAiCommunicator->from = -1;
            gameAiCommunicator->to = -1;
            gameAiCommunicator->shouldAiBeRunning = false;
            gameAiCommunicator->lock.unlock();
            return;
        }
        else
        {
            if(changeInBoard)
            {
                while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
                gameAiCommunicator->transpositionTable->clear();
                gameAiCommunicator->transpositionTable->lock.unlock();
                
                int i;
                int duplicateLength = gameAiCommunicator->duplicate.size();
                duplicate.clear();
                while(gameAiCommunicator->lock.try_lock()){}
                for(i=0; i<duplicateLength; i++)
                {
                    duplicate.push_back(gameAiCommunicator->duplicate[i]);
                }
                gameAiCommunicator->lock.unlock();
                for(i=0; i<duplicate.size(); i++)
                {
                    addHashToTranspositionTable(duplicate[i], -1, -1, -1, 100);
                }
                
                
                
                float timeAdjust = 0.90*gameStage()+0.10;     // adjusted to add extra room at end of game
                minimumTime = cpuTimeLeft/timeAdjust;
                minimumTime /= 50;                          // estimated number of moves in a game
                minimumTime /= 5;                           // adjust due to imperfect stopping
                
                startTime = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
                int openingMove = openingBook.searchRandom(&board);
                setMoves(0);
                
                openingMove = -1;
                
                if(openingMove != -1)
                {
                    // use move from opening book
                    int from = openingMove/100;
                    int to = openingMove%100;
                    
                    // chek legality; website the opening book was grabbed from has... problems
                    for(int i=0; i<moveLength[0]; i+=2)
                    {
                        if(move[i]==from && move[i+1]==to)
                        {
                            while(!gameAiCommunicator->lock.try_lock()) {}
                            gameAiCommunicator->from = from;
                            gameAiCommunicator->to = to;
                            gameAiCommunicator->shouldAiBeRunning = false;
                            gameAiCommunicator->lock.unlock();
                            return;
                        }
                    }
                }
                
                if(moveLength[0] == 2)
                {
                    while(!gameAiCommunicator->lock.try_lock()) {}
                    gameAiCommunicator->from = move[0];
                    gameAiCommunicator->to = move[1];
                    gameAiCommunicator->shouldAiBeRunning = false;
                    gameAiCommunicator->lock.unlock();
                    return;
                }
                
                currentSearchDepth = 1;
                numberOfEvals = 0;
                
                evaluator.start(&board);
                std::cout << "{" << evaluator.eval(0, 0, gameStage()) << "}\n";
            }
            // best move found
            int bestMove = search(currentSearchDepth);
            std::cout << currentSearchDepth << ": (" << boardValue << ") ";
            currentSearchDepth += 1;
            currentTime = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
            
            while(gameAiCommunicator->lock.try_lock()){}
            bool isAIBlack = gameAiCommunicator->isAIBlack;
            gameAiCommunicator->lock.unlock();
            
            // oldBoard is used to stop a glitch in the getPrincipleLine function
            Board oldBoard;
            oldBoard.set(board);
            std::vector<std::string> principleLine = getPrincipleLine(bestMove/100, bestMove%100, isAIBlack);
            board.set(oldBoard);
            std::cout << "[";
            for(int i=0; i<principleLine.size(); i++)
            {
                if(i != 0)
                {
                    std::cout << ", ";
                }
                std::cout << principleLine[i];
            }
            
            std::cout << "] ";
            std::cout << " <" << (currentTime-startTime) << ">\n";
            if(currentTime-startTime >= minimumTime)
            {
                std::cout << numberOfEvals << "==" << powf(numberOfEvals, 1/currentSearchDepth) << "\n\n";
                while(!gameAiCommunicator->lock.try_lock()) {}
                if(gameAiCommunicator->shouldAiBeRunning)
                {
                    gameAiCommunicator->from = bestMove/100;
                    gameAiCommunicator->to = bestMove%100;
                }
                gameAiCommunicator->shouldAiBeRunning = false;
                gameAiCommunicator->lock.unlock();
            }
            else
            {
                while(!gameAiCommunicator->lock.try_lock()) {}
                if(gameAiCommunicator->shouldAiBeRunning)
                {
                    gameAiCommunicator->from = bestMove/100;
                    gameAiCommunicator->to = bestMove%100;
                }
                gameAiCommunicator->lock.unlock();
            }
        }
    }
    else
    {
        // sleep
        gameAiCommunicator->lock.unlock();
        struct timespec tim, tim2;
        tim.tv_sec = 0;
        tim.tv_nsec = 10;      // microseconds
        tim.tv_nsec *= 1000;
        nanosleep(&tim , &tim2);
    }
}

int AI::search(float depth)
{
    evaluator.start(&board);
    
    setMoves(0);
    if(moveLength[0] == 0)
    {
        // no possible moves
        return -1;
    }
    
    int alpha = -1*std::numeric_limits<int>::max();
    int beta = std::numeric_limits<int>::max();
    orderMoves(0, searchTranspositionTable(), depth, alpha, beta);
    int score = -1*std::numeric_limits<int>::max();
    int value;
    int item = -1;
    
    int i = 0;
    
    while(gameAiCommunicator->lock.try_lock()){}
    int numOfThreads = gameAiCommunicator->communicator.size();
    gameAiCommunicator->lock.unlock();
    
    if(depth<3 || moveLength[0]<8 || numOfThreads <= 1)
    {
        // don't use threads
        for(i=0; i<moveLength[0]; i+=2)
        {
            value = -1*alphaBetaMakeMove(move[moveOrder[i]], move[moveOrder[i]+1], 1, depth-1, -1*beta, -1*alpha);
            
            if(value > score)
            {
                score = value;
                item = move[moveOrder[i]]*100+move[moveOrder[i]+1];
            }
            if(score>alpha)
            {
                alpha = score;
            }
        }
        boardValue = alpha;
        return item;
    }
    else
    {
        // use threads
        
        for(i=0; i<4; i+=2)
        {
            value = -1*alphaBetaMakeMove(move[moveOrder[i]], move[moveOrder[i]+1], 1, depth-1, -1*beta, -1*alpha);
            if(value > score)
            {
                score = value;
                item = move[moveOrder[i]]*100+move[moveOrder[i]+1];
            }
            if(score>alpha)
            {
                alpha = score;
            }
        }
        
        i -= 2;
        int j;
        struct timespec tim, tim2;
        tim.tv_sec = 0;
        tim.tv_nsec = 10;      // microseconds
        tim.tv_nsec *= 1000;
        
        int responses[100];
        for(j=0; j<100; j++)
        {
            responses[j] = 0;
        }
        
        while(true)
        {
            nanosleep(&tim , &tim2);
            
            while(gameAiCommunicator->lock.try_lock()){}
            if(gameAiCommunicator->shouldAiBeRunning)
            {
                gameAiCommunicator->lock.unlock();
            }
            else
            {
                gameAiCommunicator->lock.unlock();
                for(j=0; j<numOfThreads; j++)
                {
                    while(gameAiCommunicator->communicator[j]->lock.try_lock()){}
                    gameAiCommunicator->communicator[j]->levelOfSleep = 2;
                    gameAiCommunicator->communicator[j]->lock.unlock();
                }
                break;
            }
            
            
            for(j=0; j<numOfThreads; j++)
            {
                while(gameAiCommunicator->communicator[j]->lock.try_lock()){}
                if(gameAiCommunicator->communicator[j]->levelOfSleep == 2)
                {
                    // start thread from deep sleep
                    if(i+2 >= moveLength[0])
                    {
                        // done
                    }
                    else
                    {
                        // assign thread new move
                        i+=2;
                        responses[j]++;
                        gameAiCommunicator->communicator[j]->board.set(board);
                        gameAiCommunicator->communicator[j]->from = move[moveOrder[i]];
                        gameAiCommunicator->communicator[j]->to = move[moveOrder[i]+1];
                        gameAiCommunicator->communicator[j]->depthLeft = depth-1;
                        gameAiCommunicator->communicator[j]->alpha = alpha;
                        gameAiCommunicator->communicator[j]->beta = beta;
                        gameAiCommunicator->communicator[j]->levelOfSleep = 0;
                    }
                }
                else if(gameAiCommunicator->communicator[j]->levelOfSleep == 1)
                {
                    // done with last move
                    value = gameAiCommunicator->communicator[j]->value;
                    
                    responses[j]--;
                    if(value > score)
                    {
                        score = value;
                        item = gameAiCommunicator->communicator[j]->from*100+gameAiCommunicator->communicator[j]->to;
                    }
                    if(score>alpha)
                    {
                        alpha = score;
                    }
                    if(i+2 >= moveLength[0])
                    {
                        // no new info to give searcher
                        gameAiCommunicator->communicator[j]->levelOfSleep = 2;
                    }
                    else
                    {
                        // assign thread new move
                        i+=2;
                        responses[j]++;
                        gameAiCommunicator->communicator[j]->from = move[moveOrder[i]];
                        gameAiCommunicator->communicator[j]->to = move[moveOrder[i]+1];
                        gameAiCommunicator->communicator[j]->depthLeft = depth-1;
                        gameAiCommunicator->communicator[j]->alpha = alpha;
                        gameAiCommunicator->communicator[j]->beta = beta;
                        gameAiCommunicator->communicator[j]->levelOfSleep = 0;
                    }
                }
                else
                {
                    // currenting analyzing last move
                }
                gameAiCommunicator->communicator[j]->lock.unlock();
            }
            for(j=0; j<100; j++)
            {
                if(responses[j] != 0)
                {
                    break;
                }
            }
            if(j == 100)
            {
                break;
            }
        }
        
        for(j=0; j<numOfThreads; j++)
        {
            while(gameAiCommunicator->communicator[j]->lock.try_lock()){}
            gameAiCommunicator->communicator[j]->levelOfSleep = 2;
            gameAiCommunicator->communicator[j]->lock.unlock();
        }
        
        addElementToTranspositionTable(alpha, 100*move[item]+move[item+1], false, depth);
        boardValue = alpha;
        return item;
    }
}

int AI::alphaBetaTree(int layer, float depthLeft, int alpha, int beta, int endOfLastMove)
{
    while(!gameAiCommunicator->lock.try_lock()) {}
    if(!gameAiCommunicator->shouldAiBeRunning)
    {
        // stop search
        gameAiCommunicator->lock.unlock();
        return 0;
    }
    else
    {
        gameAiCommunicator->lock.unlock();
    }
    
    if(board.fiftyMoveDrawCounter == 100)
    {
        return 0;
    }
    
    Node* hash = searchTranspositionTable();
    
    if(hash != nullptr && hash->depthSearched == 100)
    {
        // three time rep
        return 0;
    }
    else if(depthLeft<=0 || layer>=18)
    {
        // evaluate
        int value = evalPosition(layer, endOfLastMove, hash, beta, depthLeft);
        return value;
    }
    
    if(layer%2 == 0)
    {
        // ai move
        setMoves(layer);
        int i;
        if(moveLength[layer] == 500*layer)
        {
            for(i=0; i<64; i++)
            {
                if(board.b[i] == 6)
                {
                    break;
                }
            }
            if(i != 64)
            {
                if(aiInCheck(i))
                {
                    // checkmate
                    return layer-1000;
                }
                else
                {
                    // stalemate
                    return 0;
                }
            }
            else
            {
                // no ai king
                return -10000;
            }
        }
        bool inCheck = false;
        for(i=0; i<64; i++)
        {
            if(board.b[i] == 6)
            {
                inCheck = aiInCheck(i);
                if(inCheck)
                {
                    depthLeft += checkExtension(depthLeft);
                    break;
                }
            }
        }
        
        int item = -1;
        int value;
        int score = -1*std::numeric_limits<int>::max();
        float bonusDepth = 0;
        int useLateMoveReduction;
        
        if(moveLength[layer]-500*layer > 30 && evaluator.getPieceCount() > 6 && depthLeft > 2 && !inCheck)
        {
            // try null move
            /*
             requirements:
              1. more than 15 legal moves
              2. more than 6 pieces on the board
              3. evaluation of board is above beta
              4. depth left is greater than 2
              5. not in check
            */
            int currentBoardValue = evalPosition(layer, endOfLastMove, hash, beta, depthLeft);
            if(currentBoardValue > beta)
            {
                currentBoardValue = -1*alphaBetaTree(layer+1, depthLeft-3, -1*beta, -1*alpha, -1);
            }
            if(currentBoardValue > beta)
            {
                return currentBoardValue;
            }
        }
        
        orderMoves(layer, hash, depthLeft, alpha, beta);
        for(i=500*layer; i<moveLength[layer]; i+=2)
        {
            if(i-500*layer <= 6)
            {
                useLateMoveReduction = false;
            }
            else
            {
                useLateMoveReduction = true;
            }
            bonusDepth = 0;
            if(board.b[move[moveOrder[i]]]==1 && move[moveOrder[i]]%8<=2)
            {
                // promotion danger extension
                bonusDepth += promotionDangerExtension(depthLeft);
                useLateMoveReduction = false;
            }
            if(board.b[move[moveOrder[i]+1]] != 0)
            {
                // capture extension
                bonusDepth += captureExtension(-1*evaluator.pieceToValue(board.b[move[moveOrder[i]]%100])-evaluator.pieceToValue(board.b[move[moveOrder[i]+1]]), depthLeft);
                useLateMoveReduction = false;
            }
            
            if(useLateMoveReduction)
            {
                bonusDepth += lateMoveReductionExtension(depthLeft);
            }
            
            value = -1*alphaBetaMakeMove(move[moveOrder[i]], move[moveOrder[i]+1], layer+1, depthLeft+bonusDepth-1, -1*beta, -1*alpha);
            
            if(value>score)
            {
                score = value;
                item = moveOrder[i];
            }
            if(score > alpha)
            {
                alpha = score;
            }
            if(alpha >= beta)
            {
                killer[layer][1] = killer[layer][0];
                killer[layer][0] = move[moveOrder[i]]*100+move[moveOrder[i]+1];
                break;
            }
        }
        if(i == moveLength[layer])
        {
            addElementToTranspositionTable(alpha, 100*move[item]+move[item+1], false, depthLeft);
        }
        else
        {
            addElementToTranspositionTable(alpha, 100*move[item]+move[item+1], true, depthLeft);
        }
        return alpha;
    }
    else
    {
        // human move
        int value;
        int score = -1*std::numeric_limits<int>::max();
        int item = -1;
        setMoves(layer);
        int i;
        if(moveLength[layer] == 500*layer)
        {
            for(i=0; i<64; i++)
            {
                if(board.b[i] == -6)
                {
                    break;
                }
            }
            if(i != 64)
            {
                if(humanInCheck(i))
                {
                    // checkmate
                    return layer-1000;
                }
                else
                {
                    // stalemate
                    return 0;
                }
            }
            else
            {
                // no human king
                return -10000;
            }
        }
        bool inCheck = false;
        for(i=0; i<64; i++)
        {
            if(board.b[i] == -6)
            {
                inCheck = humanInCheck(i);
                if(inCheck)
                {
                    depthLeft += checkExtension(depthLeft);
                    break;
                }
            }
        }
        
        if(moveLength[layer]-500*layer > 30 && evaluator.getPieceCount() > 6 && depthLeft > 2 && !inCheck)
        {
            // try null move
            /*
             requirements:
             1. more than 15 legal moves
             2. more than 6 pieces on the board
             3. evaluation of board is above beta
             4. depth left is greater than 2
             5. not in check
             */
            int currentBoardValue = evalPosition(layer, endOfLastMove, hash, beta, depthLeft);
            if(currentBoardValue > beta)
            {
                currentBoardValue = -1*alphaBetaTree(layer+1, depthLeft-3, -1*beta, -1*alpha, -1);
            }
            if(currentBoardValue > beta)
            {
                return currentBoardValue;
            }
        }
        
        orderMoves(layer, hash, depthLeft, alpha, beta);
        float bonusDepth;
        bool useLateMoveReduction;
        
        for(i=500*layer; i<moveLength[layer]; i+=2)
        {
            if(i-500*layer <= 6)
            {
                useLateMoveReduction = false;
            }
            else
            {
                useLateMoveReduction = true;
            }
            
            bonusDepth = 0;
            if(board.b[move[moveOrder[i]]]==1 && move[moveOrder[i]]%8<=2)
            {
                bonusDepth += promotionDangerExtension(depthLeft);
                useLateMoveReduction = false;
            }
            if(board.b[move[moveOrder[i]+1]] != 0)
            {
                // capture extension
                bonusDepth += captureExtension(evaluator.pieceToValue(board.b[move[moveOrder[i]+1]])+evaluator.pieceToValue(board.b[move[moveOrder[i]]%100]), depthLeft);
                useLateMoveReduction = false;
            }
            
            if(useLateMoveReduction)
            {
                bonusDepth += lateMoveReductionExtension(depthLeft);
            }
            bonusDepth = 0;
            if(board.b[move[moveOrder[i]]]==-1 && move[moveOrder[i]]%8>=6)
            {
                bonusDepth += promotionDangerExtension(depthLeft);
            }
            
            value = -1*alphaBetaMakeMove(move[moveOrder[i]], move[moveOrder[i]+1], layer+1, depthLeft+bonusDepth-1, -1*beta, -1*alpha);
            
            if(value>score)
            {
                score = value;
                item = moveOrder[i];
            }
            if(score>alpha)
            {
                alpha = score;
            }
            if(alpha >= beta)
            {
                killer[layer][1] = killer[layer][0];
                killer[layer][0] = move[moveOrder[i]]*100+move[moveOrder[i]+1];
                break;
            }
        }
        
        
        addElementToTranspositionTable(alpha, 100*move[item]+move[item+1], false, depthLeft);
        return alpha;
    }
}

float AI::moveEval(int from, int to, int layer)
{
    int value;
    int sto, stoB;
    if(board.b[from%100]>0)
    {
        // cpu
        if(from>=100)
        {
            // promotion
            sto = board.b[to];
            board.b[to] = (int) (from/100);
            board.b[from%100] = 0;
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
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
            
            
            value = evalPosition(layer, to, searchTranspositionTable(), INFINITY, 0);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            board.fiftyMoveDrawCounter = stoB;
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
            
            value = evalPosition(layer, to, searchTranspositionTable(), INFINITY, 0);
            
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
            
            value = evalPosition(layer, to, searchTranspositionTable(), INFINITY, 0);
            
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
            
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
            value = evalPosition(layer, to, searchTranspositionTable(), INFINITY, 0);
            
            board.fiftyMoveDrawCounter = stoB;
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
            board.b[to] *= -1;
            board.b[from%100] = 0;
            stoB = board.fiftyMoveDrawCounter;
            
            if(sto == 0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
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
            
            value = evalPosition(layer, to, searchTranspositionTable(), INFINITY, 0);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            
            board.fiftyMoveDrawCounter = stoB;
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
            
            value = evalPosition(layer, to, searchTranspositionTable(), INFINITY, 0);
            
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
            
            value = evalPosition(layer, to, searchTranspositionTable(), INFINITY, 0);
            
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
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
            value = evalPosition(layer, to, searchTranspositionTable(), INFINITY, 0);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[from] = board.b[to];
            board.b[to] = sto;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
        }
    }
    return value;
}

float AI::moveOrderSearch(int from, int to, int layer, float depthToSearch)
{
    return -1*alphaBetaMakeMove(from, to, layer, depthToSearch, -1*std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
}

void AI::orderMoves(int layer, Node* hash, float depthLeft, int alpha, int beta)
{
    /*
     1000       PV-node
     900 ± 10   "good" captures
     800 - 801  killer
     0          equal captures
     -1 - -10   "bad" captures
     -100       other
     */
    int sto;
    if(layer%2 == 0)
    {
        // cpu
        if(depthLeft>=minimumDepthLeftForDetailedMoveOrdering)
        {
            for(int i=500*layer; i<moveLength[layer]; i+=2)
            {
                moveOrder[i] = i;
                if(hash && hash->bestMove == 100*move[i]+move[i+1])
                {
                    evalOrder[i] = 1000;
                }
                else if(100*move[i]+move[i+1] == killer[layer][0] && board.b[move[i+1]] == 0)
                {
                    evalOrder[i] = 801;
                }
                else if(100*move[i]+move[i+1] == killer[layer][1] && board.b[move[i+1]] == 0)
                {
                    evalOrder[i] = 800;
                }
                else
                {
                    evalOrder[i] = moveOrderSearch(move[i], move[i+1], layer+1, depthLeft+depthDiffForMoveOrdering);
                }
                if(board.b[move[i]] == 1)
                {
                    // pawn-move searched later
                    evalOrder[i] -= 0.01;
                }
            }
        }
        else
        {
            for(int i=500*layer; i<moveLength[layer]; i+=2)
            {
                moveOrder[i] = i;
                if(hash && hash->bestMove == 100*move[i]+move[i+1])
                {
                    evalOrder[i] = 1000;
                }
                else if(100*move[i]+move[i+1] == killer[layer][0] && board.b[move[i+1]] == 0)
                {
                    evalOrder[i] = 801;
                }
                else if(100*move[i]+move[i+1] == killer[layer][1] && board.b[move[i+1]] == 0)
                {
                    evalOrder[i] = 800;
                }
                else if(board.b[move[i+1]])
                {
                    // capture
                    if(move[i+1] == board.lastMove[1])
                    {
                        // recapture
                        evalOrder[i] = 100 - board.b[move[i]%100];
                    }
                    else
                    {
                        evalOrder[i] = -1*evaluator.pieceToValue(board.b[move[i+1]]) - evaluator.pieceToValue(board.b[move[i]]);
                        if(evalOrder[i] > 0)
                        {
                            evalOrder[i] += 900;
                        }
                    }
                }
                else
                {
                    evalOrder[i] = -100;
                }
                if(board.b[move[i]] == 1)
                {
                    // pawn-move searched later
                    evalOrder[i] -= 0.01;
                }
            }
        }
    }
    else
    {
        // human
        if(depthLeft>=minimumDepthLeftForDetailedMoveOrdering)
        {
            for(int i=500*layer; i<moveLength[layer]; i+=2)
            {
                moveOrder[i] = i;
                
                if(hash && hash->bestMove == 100*move[i]+move[i+1])
                {
                    evalOrder[i] = 1000;
                }
                else if(100*move[i]+move[i+1] == killer[layer][0] && board.b[move[i+1]] == 0)
                {
                    evalOrder[i] = 801;
                }
                else if(100*move[i]+move[i+1] == killer[layer][1] && board.b[move[i+1]] == 0)
                {
                    evalOrder[i] = 800;
                }
                else
                {
                    evalOrder[i] = moveOrderSearch(move[i], move[i+1], layer+1, depthLeft+depthDiffForMoveOrdering);
                    // evalOrder[i] = moveEval(move[i], move[i+1], layer);
                }
                if(board.b[move[i]] == 1)
                {
                    // pawn-move searched later
                    evalOrder[i] -= 0.01;
                }
            }
        }
        else
        {
            for(int i=500*layer; i<moveLength[layer]; i+=2)
            {
                moveOrder[i] = i;
                if(hash && hash->bestMove == 100*move[i]+move[i+1])
                {
                    evalOrder[i] = 1000;
                }
                else if(100*move[i]+move[i+1] == killer[layer][0] && board.b[move[i+1]] == 0)
                {
                    evalOrder[i] = 801;
                }
                else if(100*move[i]+move[i+1] == killer[layer][1] && board.b[move[i+1]] == 0)
                {
                    evalOrder[i] = 800;
                }
                else if(board.b[move[i+1]])
                {
                    // capture
                    if(move[i+1] == board.lastMove[1])
                    {
                        // recapture
                        evalOrder[i] = 100 + board.b[move[i]%100];
                    }
                    else
                    {
                        evalOrder[i] = evaluator.pieceToValue(board.b[move[i+1]]) + evaluator.pieceToValue(board.b[move[i]]);
                        if(evalOrder[i] > 0)
                        {
                            // good capture
                            evalOrder[i] += 900;
                        }
                    }
                }
                else
                {
                    evalOrder[i] = -100;
                }
                if(board.b[move[i]] == 1)
                {
                    // pawn-move searched later
                    evalOrder[i] -= 0.01;
                }
            }
        }
    }
    
    int counter;
    if(layer%2 == 0)
    {
        for(int i=500*layer; i<moveLength[layer]; i+=2)
        {
            counter = 0;
            for(int j=500*layer+2; j<moveLength[layer]; j+=2)
            {
                if(evalOrder[moveOrder[j-2]] < evalOrder[moveOrder[j]])
                {
                    counter++;
                    sto = moveOrder[j-2];
                    moveOrder[j-2] = moveOrder[j];
                    moveOrder[j] = sto;
                }
            }
            if(counter == 0)
            {
                break;
            }
        }
    }
    else
    {
        for(int i=500*layer; i<moveLength[layer]; i+=2)
        {
            counter = 0;
            for(int j=500*layer+2; j<moveLength[layer]; j+=2)
            {
                if(evalOrder[moveOrder[j-2]] < evalOrder[moveOrder[j]])
                {
                    counter++;
                    sto = moveOrder[j-2];
                    moveOrder[j-2] = moveOrder[j];
                    moveOrder[j] = sto;
                }
            }
            if(counter == 0)
            {
                break;
            }
        }
    }
}

Node* AI::searchTranspositionTable()
{
    while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
    Node* node = gameAiCommunicator->transpositionTable->searchFor(board.getZorbistHash());
    gameAiCommunicator->transpositionTable->lock.unlock();
    return node;
}

int AI::alphaBetaMakeMove(int from, int to, int layer, float depthLeft, int alpha, int beta)
{
    evaluator.makeMove(from, to, layer, &board);
    int sto = 0;
    int stoB;
    int value;
    
    if(layer%2==1)
    {
        // cpu
        if(from>=100)
        {
            // promotion
            sto = board.b[to];
            board.b[to] = (int) (from/100);
            board.b[from%100] = 0;
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
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
            
            value = alphaBetaTree(layer, depthLeft, alpha, beta, to);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            board.fiftyMoveDrawCounter = stoB;
            board.b[to] = sto;
            board.b[from%100] = 1;
        }
        else if(board.b[from]==6 && (to-from==16 || from-to==16))
        {
            // castle
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            board.castling[0] = 0;
            board.castling[1] = 0;
            board.fiftyMoveDrawCounter++;
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
            
            value = alphaBetaTree(layer, depthLeft, alpha, beta, to);
            
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
            sto = 1;
            board.b[to+1] = 0;
            board.b[to] = 1;
            board.b[from] = 0;
            stoB = board.fiftyMoveDrawCounter;
            board.fiftyMoveDrawCounter = 0;
            
            value = alphaBetaTree(layer, depthLeft, alpha, beta, to);
            
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
            
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
            value = alphaBetaTree(layer, depthLeft, alpha, beta, to);
            
            board.fiftyMoveDrawCounter = stoB;
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
            board.b[to] *= -1;
            board.b[from%100] = 0;
            stoB = board.fiftyMoveDrawCounter;
            
            if(sto == 0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
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
            
            value = alphaBetaTree(layer, depthLeft, alpha, beta, to);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[to] = sto;
            board.b[from%100] = -1;
        }
        else if(board.b[from]==-6 && (to-from==16 || from-to==16))
        {
            // castling
            int sto0 = board.castling[2];
            int sto1 = board.castling[3];
            board.castling[2] = 0;
            board.castling[3] = 0;
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
            
            value = alphaBetaTree(layer, depthLeft, alpha, beta, to);
            
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
            sto = -1;
            board.b[to-1] = 0;
            board.b[to] = -1;
            board.b[from] = 0;
            stoB = board.fiftyMoveDrawCounter;
            board.fiftyMoveDrawCounter = 0;
            
            value = alphaBetaTree(layer, depthLeft, alpha, beta, to);
            
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
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
            value = alphaBetaTree(layer, depthLeft, alpha, beta, to);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[from] = board.b[to];
            board.b[to] = sto;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
        }
    }
    evaluator.undoMove(from, to, layer, &board, sto);
    
    return value;
}

void AI::addElementToTranspositionTable(int value, int bestMove, int betaCutoff, float depthSearched)
{
    Node* node = searchTranspositionTable();
    if(node)
    {
        if(node->depthSearched != 100)
        {
            uint64_t key = board.getZorbistHash();
            while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
            gameAiCommunicator->transpositionTable->insert(key, bestMove, depthSearched);
            gameAiCommunicator->transpositionTable->lock.unlock();
        }
    }
    else
    {
        uint64_t key = board.getZorbistHash();
        while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
        gameAiCommunicator->transpositionTable->insert(key, bestMove, depthSearched);
        gameAiCommunicator->transpositionTable->lock.unlock();
    }
}

int AI::evalPosition(int layer, int to, Node* node, int beta, float depthToSearch)
{
    return evaluator.eval(layer, to, gameStage());
}

float AI::gameStage()
{
    float cpu = 0;
    float human = 0;
    for(int i=0; i<64; i++)
    {
        if(board.b[i] == 1)
        {
            cpu += 1.8;
        }
        else if(board.b[i] == -1)
        {
            human += 1.8;
        }
        else if(board.b[i]>0 && board.b[i]!=6)
        {
            cpu += 2.8;
        }
        else if(board.b[i]<0 && board.b[i]!=-6)
        {
            human += 2.8;
        }
    }
    cpu += 6;
    if(board.b[15]!=2)
    {
        cpu -= 1;
    }
    if(board.b[23]!=3)
    {
        cpu -= 1;
    }
    if(board.b[47]!=3)
    {
        cpu -= 1;
    }
    if(board.b[55]!=2)
    {
        cpu -= 1;
    }
    if(board.b[30]!=1)
    {
        cpu -= 1;
    }
    if(board.b[38]!=1)
    {
        cpu -= 1;
    }
    
    human += 6;
    if(board.b[8]!=-2)
    {
        human -= 1;
    }
    if(board.b[16]!=-3)
    {
        human -= 1;
    }
    if(board.b[40]!=-3)
    {
        human -= 1;
    }
    if(board.b[48]!=-2)
    {
        human -= 1;
    }
    if(board.b[25]!=-1)
    {
        human -= 1;
    }
    if(board.b[33]!=-1)
    {
        human -= 1;
    }
    
    cpu += 10;
    human += 10;
    
    cpu /= 50;
    human /= 50;
    
    if(human<cpu)
    {
        return human;
    }
    else
    {
        return  cpu;
    }
}

float AI::checkExtension(float depthLeft)
{
    return 0;
    return 0.8;
}

float AI::promotionDangerExtension(float depthLeft)
{
    return 0;
    return 0.8;
}

float AI::captureExtension(float captureDiff, float depthLeft)
{
    return 0;
    if(captureDiff == 0)
    {
        return 0.5;
    }
    else
    {
        return -0.5;
    }
}

float AI::lateMoveReductionExtension(float depthLeft)
{
    if(depthLeft > 3)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int AI::numberOfPieceOnBoard(int piece)
{
    int counter = 0;
    for(int i=0; i<64; i++)
    {
        if(board.b[i] == piece)
        {
            counter++;
        }
    }
    return counter;
}