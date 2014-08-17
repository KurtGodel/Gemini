//
//  AI.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "AI.h"

AI::AI(RunAIInput* gameAiCommunicator)
:evaluator(&board), gameAiCommunicator(gameAiCommunicator), movementCalculator(board)
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
    
    Move m;
    m.from = from;
    m.to = to;
    getPrincipleLineMakeMove(m, 1, &rtn, isAIBlack);
    
    return rtn;
}

void AI::getPrincipleLineTree(int layer, std::vector<std::string>* rtn, bool isAIBlack)
{
    if(layer > 20)
    {
        return;
        // stop (probably in infinite loop)
    }
    Node* node = searchTranspositionTable();
    if(node == nullptr)
    {
        // done
    }
    else
    {
        if(node->bestMove != -1)
        {
            Move m;
            m.from = node->bestMove/100;
            m.to = node->bestMove%100;
            if(isAIBlack)
            {
                rtn->push_back(moveConverter.convertMoveToString(&board, m.from, m.to, 1));
            }
            else
            {
                rtn->push_back(moveConverter.convertMoveToString(&board, m.from, m.to, 0));
            }
            getPrincipleLineMakeMove(m, layer+1, rtn, isAIBlack);
        }
        else
        {
            // done
        }
    }
}

void AI::getPrincipleLineMakeMove(Move m, int layer, std::vector<std::string>* rtn, bool isAIBlack)
{
    board.makeMove(m);
    getPrincipleLineTree(layer, rtn, isAIBlack);
    board.unmakeMove();
}

void AI::addHashToTranspositionTable(uint64_t hash, int bestMove, int value, int betaSearchedAt, float depthSearched, bool searchCompleted)
{
    while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
    gameAiCommunicator->transpositionTable->insert(hash, bestMove, value, betaSearchedAt, depthSearched, searchCompleted);
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
        useOpeningBook = gameAiCommunicator->useOpeningBook;
        useLateMoveReductions = gameAiCommunicator->useLateMoveReductions;
        useNullMove = gameAiCommunicator->useNullMove;
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
            if(changeInBoard) // beginning new search
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
                    addHashToTranspositionTable(duplicate[i], -1, -1, -1, 100, true);
                }
                
                
                float timeAdjust = 0.90*gameStage()+0.10;     // adjusted to add extra room at end of game
                minimumTime = cpuTimeLeft/timeAdjust;
                minimumTime /= 50;                          // estimated number of moves in a game
                minimumTime /= 2;                         // adjust due to imperfect stopping
                
                maximumTime = 8*minimumTime;
                
                startTime = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
                int openingMove;
                
                if(useOpeningBook)
                {
                    openingMove = openingBook.searchRandom(&board);
                }
                else
                {
                    openingMove = -1;
                }
                setMoves(0);
                
                if(openingMove != -1)
                {
                    // use move from opening book
                    int from = openingMove/100;
                    int to = openingMove%100;
                    
                    // chek legality; website the opening book was grabbed from has... problems
                    for(int i=0; i<moveLength[0]; i++)
                    {
                        if(move[i].from==from && move[i].to==to)
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
                
                if(moveLength[0] == 1)
                {
                    while(!gameAiCommunicator->lock.try_lock()) {}
                    gameAiCommunicator->from = move[0].from;
                    gameAiCommunicator->to = move[0].to;
                    gameAiCommunicator->shouldAiBeRunning = false;
                    gameAiCommunicator->lock.unlock();
                    return;
                }
                
                currentSearchDepth = 1;
                numberOfEvals = 0;
                
                evaluator.start(&board);
                float score = evaluator.eval(0, 0);
                std::cout << "{" << 1/(1+powf(2.718281828, score/-1000)) << "}\n";
            }
            // best move found
            int bestMove = search(currentSearchDepth);
                        
            float logisticScore = boardValue;
            std::cout << currentSearchDepth << ": (" << 1/(1+powf(2.718281828, logisticScore/-1000)) << " = " << logisticScore << ") ";
            currentSearchDepth += depthStepSize;
            currentTime = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
            
            while(gameAiCommunicator->lock.try_lock()){}
            bool isAIBlack = gameAiCommunicator->isAIBlack;
            gameAiCommunicator->lock.unlock();
            
            // oldBoard is used to stop a glitch in the getPrincipleLine function
            
            
            Board oldBoard;
            oldBoard.set(board);
            std::vector<std::string> principleLine = getPrincipleLine(bestMove/100, bestMove%100, isAIBlack);
            // std::vector<std::string> principleLine = getPrincipleLine(30, 29, isAIBlack);
            board.set(oldBoard);
            std::cout << bestMove << "\n";
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
            while(!gameAiCommunicator->lock.try_lock()) {}
            if(currentTime-startTime >= minimumTime || currentSearchDepth-depthStepSize >= maxDepthSearch)
            {
                std::cout << numberOfEvals << "==" << powf(numberOfEvals, 1/currentSearchDepth) << "\n\n";
                if(gameAiCommunicator->shouldAiBeRunning)
                {
                    gameAiCommunicator->from = bestMove/100;
                    gameAiCommunicator->to = bestMove%100;
                }
                gameAiCommunicator->shouldAiBeRunning = false;
            }
            else if(gameAiCommunicator->shouldAiBeRunning)
            {
                // last search was not interrupted
                gameAiCommunicator->from = bestMove/100;
                gameAiCommunicator->to = bestMove%100;
            }
            gameAiCommunicator->lock.unlock();
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
    nodeCount = 0;
    evaluator.start(&board);
    
    setMoves(0);
    if(moveLength[0] == 0)
    {
        // no possible moves
        return -1;
    }
    
    bool inCheck = false;
    for(int i=0; i<64; i++)
    {
        if(board.b[i] == 6)
        {
            inCheck = aiInCheck(i);
            break;
        }
    }
    Node* hash = searchTranspositionTable();
    
    int alpha = -1*std::numeric_limits<int>::max();
    int beta = std::numeric_limits<int>::max();
    orderMoves(0, hash, depth, alpha, beta);
    int score = -1*std::numeric_limits<int>::max();
    int value;
    int item = -1;
    float bonusDepth = 0;
    
    int i = 0;
    
    parentZobrist[0] = board.getZorbistHash();
    
    // don't use threads
    for(i=0; i<moveLength[0]; i++)
    {
        bonusDepth = getExtension(0, move[moveOrder[i]], depth, inCheck, i);
        value = -1*alphaBetaMakeMove(move[moveOrder[i]], 1, depth+bonusDepth-1, -1*beta, -1*alpha);
        
        if(attemptedLateMoveReduction[0] != 0 && value > score)
        {
            value = -1*alphaBetaMakeMove(move[moveOrder[i]], 1, depth+bonusDepth-1-attemptedLateMoveReduction[0], -1*beta, -1*alpha);
        }
        
        if(value > score)
        {
            score = value;
            item = move[moveOrder[i]].from*100+move[moveOrder[i]].to;
        }
        if(score > alpha)
        {
            alpha = score;
        }
    }
    
    addElementToTranspositionTable(item, alpha, beta, depth, true);
    boardValue = alpha;
    return item;
}

bool AI::checkForCycle(int layer)
{
    for(int i=0; i<layer; i++)
    {
        if(parentZobrist[i] == parentZobrist[layer])
        {
            return true;
        }
    }
    return false;
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
    if(hash != nullptr && hash->depthSearched >= depthLeft && hash->bestMove != -1 && hash->searchCompleted && beta <= hash->betaSearchedAt)
    {
        return hash->value;
    }
    
    if(depthLeft <= 0 || layer >= maxNumberOfPly-3)        // gives an irrational cushion to prevent running out of memory in the array
    {
        // evaluate
        return evalPosition(layer, endOfLastMove, hash, beta, depthLeft);
    }
    
    parentZobrist[layer] = board.getZorbistHash();
    
    if(checkForCycle(layer))
    {
        // this position occured earlier in the search - this indicates that a draw will probably be forced by 3-time rep
        return 0;
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
                    return layer-1000000000;
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
                break;
            }
        }
        
        int item = -1;
        int value;
        int score = -1*std::numeric_limits<int>::max();
        float bonusDepth = 0;
        
        if(!inCheck && moveLength[layer]-500*layer > minimumLegalMovesToNullMovePrune && evaluator.getPieceCount() > minimumPiecesToNullMovePrune && useNullMove && depthLeft > nullR)
        {
            // try null move
            /*
             requirements:
             1. more than minimumLegalMovesToNullMovePrune legal moves
             2. more than minimumPiecesToNullMovePrune pieces on the board
             3. evaluation of board is above beta
             4. not in check
            */
            int currentBoardValue = evalPosition(layer, endOfLastMove, hash, beta, depthLeft);
            if(currentBoardValue >= beta)
            {
                currentBoardValue = -1*alphaBetaTree(layer+1, depthLeft-nullR, -1*beta, -1*alpha, -1);
            }
            if(currentBoardValue > beta)
            {
                return currentBoardValue;
            }
        }
        
        orderMoves(layer, hash, depthLeft, alpha, beta);
        
        for(i=500*layer; i<moveLength[layer]; i++)
        {
            bonusDepth = getExtension(layer, move[moveOrder[i]], depthLeft, inCheck, i-500*layer);
            value = -1*alphaBetaMakeMove(move[moveOrder[i]], layer+1, depthLeft+bonusDepth-1, -1*beta, -1*alpha);
            
            if(value > score)
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
                killer[layer][0] = move[moveOrder[i]].from*100+move[moveOrder[i]].to;
                break;
            }
        }
        if(i == moveLength[layer])
        {
            addElementToTranspositionTable(100*move[item].from+move[item].to, alpha, beta, depthLeft, true);
        }
        else
        {
            addElementToTranspositionTable(100*move[item].from+move[item].to, alpha, beta, depthLeft, false);
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
                    return layer-1000000000;
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
                break;
            }
        }
        
        if(!inCheck && moveLength[layer]-500*layer > minimumLegalMovesToNullMovePrune && evaluator.getPieceCount() > minimumPiecesToNullMovePrune && useNullMove && depthLeft > nullR)
        {
            // try null move
            /*
             requirements:
             1. more than minimumLegalMovesToNullMovePrune legal moves
             2. more than minimumPiecesToNullMovePrune pieces on the board
             3. evaluation of board is above beta
             4. not in check
            */
            int currentBoardValue = evalPosition(layer, endOfLastMove, hash, beta, depthLeft);
            if(currentBoardValue >= beta)
            {
                currentBoardValue = -1*alphaBetaTree(layer+1, depthLeft-nullR, -1*beta, -1*alpha, -1);
            }
            if(currentBoardValue > beta)
            {
                return currentBoardValue;
            }
        }
        
        orderMoves(layer, hash, depthLeft, alpha, beta);
        float bonusDepth;
        
        for(i=500*layer; i<moveLength[layer]; i++)
        {
            bonusDepth = getExtension(layer, move[moveOrder[i]], depthLeft, inCheck, i-500*layer);
            value = -1*alphaBetaMakeMove(move[moveOrder[i]], layer+1, depthLeft+bonusDepth-1, -1*beta, -1*alpha);
            
            if(attemptedLateMoveReduction[layer] != 0 && value > score)
            {
                // this is a PV-node! re-search this line at full depth
                value = -1*alphaBetaMakeMove(move[moveOrder[i]], layer+1, depthLeft+bonusDepth-1-attemptedLateMoveReduction[layer], -1*beta, -1*alpha);
            }
            
            if(value > score)
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
                killer[layer][0] = move[moveOrder[i]].from*100+move[moveOrder[i]].to;
                break;
            }
        }
        
        if(i == moveLength[layer])
        {
            addElementToTranspositionTable(100*move[item].from+move[item].to, alpha, beta, depthLeft, true);
        }
        else
        {
            addElementToTranspositionTable(100*move[item].from+move[item].to, alpha, beta, depthLeft, false);
        }
        
        return alpha;
    }
}

float AI::moveEval(Move m, int layer)
{
    int pieceTaken = evaluator.makeMove(m.from, m.to, layer, &board);
    board.makeMove(m);
    int value = evalPosition(layer, m.to, searchTranspositionTable(), INFINITY, 0);
    board.unmakeMove();
    evaluator.undoMove(m.from, m.to, layer, &board, pieceTaken);
    return value;
}

float AI::moveOrderSearch(Move m, int layer, float depthToSearch)
{
    return -1*alphaBetaMakeMove(m, layer, depthToSearch, -1*std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
}

void AI::orderMoves(int layer, Node* hash, float depthLeft, int alpha, int beta)
{
    /*
     1000       PV-node
     952-955    promotions
     900 ± 10   "good" captures
     800 ± 10   recaptures
     700 - 701  killer
     100 ± 10   equal captures
     -1 - -10   "bad" captures
     -100±10ish other
     */
    if(layer%2 == 0)
    {
        // cpu
        for(int i=500*layer; i<moveLength[layer]; i++)
        {
            moveOrder[i] = i;
            if(hash && hash->bestMove == 100*move[i].from+move[i].to)
            {
                evalOrder[i] = 1000;
            }
            else if(move[i].from >= 100)
            {
                // promotion
                evalOrder[i] = 900+move[i].from/100;
            }
            else if(100*move[i].from+move[i].to == killer[layer][0] && board.b[move[i].to] == 0)
            {
                evalOrder[i] = 701;
            }
            else if(100*move[i].from+move[i].to == killer[layer][1] && board.b[move[i].to] == 0)
            {
                evalOrder[i] = 700;
            }
            else if(board.b[move[i].to] != 0)
            {
                // capture
                if(move[i].to == board.lastMove[1])
                {
                    // recapture
                    evalOrder[i] = 800 - board.b[move[i].from%100];
                }
                else
                {
                    evalOrder[i] = -1*evaluator.pieceToValue(board.b[move[i].to]) - evaluator.pieceToValue(board.b[move[i].from]);
                    if(evalOrder[i] > 0)
                    {
                        // good captures
                        evalOrder[i] += 900;
                    }
                    else if(evalOrder[i] == 0)
                    {
                        // neutral capture
                        if(depthLeft > 6)
                        {
                            evalOrder[i] = 100+staticExchangeEvaluation(move[i], layer);
                        }
                    }
                }
            }
            else
            {
                evalOrder[i] = -100+normalMoveBasicOrderEval(move[i]);
            }
        }
    }
    else
    {
        // human
        
        for(int i=500*layer; i<moveLength[layer]; i++)
        {
            moveOrder[i] = i;
            if(hash && hash->bestMove == 100*move[i].from+move[i].to)
            {
                evalOrder[i] = 1000;
            }
            else if(move[i].from >= 100)
            {
                // promotion
                evalOrder[i] = 900+move[i].from/100;
            }
            else if(100*move[i].from+move[i].to == killer[layer][0] && board.b[move[i].to] == 0)
            {
                evalOrder[i] = 701;
            }
            else if(100*move[i].from+move[i].to == killer[layer][1] && board.b[move[i].to] == 0)
            {
                evalOrder[i] = 700;
            }
            else if(board.b[move[i].to] != 0)
            {
                // capture
                if(move[i].to == board.lastMove[1])
                {
                    // recapture
                    evalOrder[i] = 800 + board.b[move[i].from%100];
                }
                else
                {
                    evalOrder[i] = evaluator.pieceToValue(board.b[move[i].to]) + evaluator.pieceToValue(board.b[move[i].from]);
                    if(evalOrder[i] > 0)
                    {
                        // good capture
                        evalOrder[i] += 900;
                    }
                    else if(evalOrder[i] == 0)
                    {
                        // neutral capture
                        if(depthLeft > 6)
                        {
                            evalOrder[i] = 100+staticExchangeEvaluation(move[i], layer);
                        }
                    }
                }
            }
            else
            {
                evalOrder[i] = -100+normalMoveBasicOrderEval(move[i]);
            }
        }
    }
    
    
    // sort moveOrder by evalOrder[moveOrder[i]]

//    quickSortIndeciesLargeToSmall(moveOrder, evalOrder, 500 * layer, moveLength[layer] - 1);
    
    int sto;
    int counter;
    if(layer%2 == 0)
    {
        for(int i=500*layer; i<moveLength[layer]; i++)
        {
            counter = 0;
            for(int j=500*layer+1; j<moveLength[layer]; j++)
            {
                if(evalOrder[moveOrder[j-1]] < evalOrder[moveOrder[j]])
                {
                    counter++;
                    sto = moveOrder[j-1];
                    moveOrder[j-1] = moveOrder[j];
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
        for(int i=500*layer; i<moveLength[layer]; i++)
        {
            counter = 0;
            for(int j=500*layer+1; j<moveLength[layer]; j++)
            {
                if(evalOrder[moveOrder[j-1]] < evalOrder[moveOrder[j]])
                {
                    counter++;
                    sto = moveOrder[j-1];
                    moveOrder[j-1] = moveOrder[j];
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

float AI::normalMoveBasicOrderEval(Move m)
{
    return 0;
}

Node* AI::searchTranspositionTable()
{
    while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
    Node* node = gameAiCommunicator->transpositionTable->searchFor(board.getZorbistHash());
    gameAiCommunicator->transpositionTable->lock.unlock();
    return node;
}

int AI::alphaBetaMakeMove(Move m, int layer, float depthLeft, int alpha, int beta)
{
    if(abs(board.b[m.to]) == 6)
    {
        // TODO
    }
    
    int pieceTaken = evaluator.makeMove(m.from, m.to, layer, &board);
    board.makeMove(m);
    int value = alphaBetaTree(layer, depthLeft, alpha, beta, m.to);
    board.unmakeMove();
    evaluator.undoMove(m.from, m.to, layer, &board, pieceTaken);
    return value;
}

void AI::addElementToTranspositionTable(int bestMove, int value, int betaSearchedAt, float depthSearched, bool searchCompleted)
{
    while(gameAiCommunicator->transpositionTable->lock.try_lock()){}
    gameAiCommunicator->transpositionTable->insert(board.getZorbistHash(), bestMove, value, betaSearchedAt, depthSearched, searchCompleted);
    gameAiCommunicator->transpositionTable->lock.unlock();
}

int AI::evalPosition(int layer, int to, Node* node, int beta, float depthToSearch)
{
    nodeCount++;
    
    if(nodeCount%10000 == 0)
    {
        // fail-safe stop search due to tree-explosion
        int currentTime = std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
        if(currentTime-startTime > maximumTime)
        {
            while(gameAiCommunicator->lock.try_lock()){};
            gameAiCommunicator->shouldAiBeRunning = false;
            gameAiCommunicator->lock.unlock();
        }
    }
    
    if(layer%2 == 0)
    {
        return evaluator.eval(layer, to);
    }
    else
    {
        return -1*evaluator.eval(layer, to);
    }
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

float AI::staticExchangeEvaluation(Move m, int layer)
{
    // is this move safe?
    int from = m.from%100;
    int sto = board.b[m.to];
    board.b[m.to] = board.b[from];
    board.b[from] = 0;
    
    // calculate stuff
    float rtn = evaluator.staticExchangeEvaluation(m.to, layer);
    
    board.b[from] = board.b[m.to];
    board.b[m.to] = sto;
    return rtn;
}

float AI::getExtension(int layer, Move m, float depthLeft, bool inCheck, int rank)
{
    attemptedLateMoveReduction[layer] = 0;
    if(inCheck)
    {
        // currently in check
        return 0;
    }
    else
    {
        // not in check
        if(m.from > 100)
        {
            // promotion
            return 0;
        }
        else if(abs(board.b[m.from]) == 1 && (m.to%8 == 1 || m.to%8 == 6))
        {
            // near promotion
            return 0;
        }
        else
        {
            // other
            if(board.b[m.to] != 0)
            {
                // capture
                return 0;
            }
            else
            {
                // non-capture
                if(useLateMoveReductions && moveLength[layer]-500*layer > minimumLegalMovesToLMR)
                {
                    // use LMR (late move reduction)
                    // Note: we only use LMR if we have at more than 15 legal moves, are not in check, and the last move wasn't a capture or promotion
                    if(rank < numberOfNonLateMoveReductions)
                    {
                        // rank: 0, 1, 2, 3
                        return 0;
                    }
                    else
                    {
                        attemptedLateMoveReduction[layer] = -1;
                        return -1;
                    }
                }
                else
                {
                    // do not use LMR
                    return 0;
                }
            }
        }
    }
}

bool AI::boardEquality(char* arr)
{
    for(int i=0; i<64; i++)
    {
        if(arr[i] != board.b[i])
        {
            return false;
        }
    }
    return true;
}

bool AI::debugFunction()
{
    return false;
}

void AI::quickSortIndeciesLargeToSmall(int* indecies, float* values, int start, int pivot) {
    
    int swapIndex;
    double swapValue;
    
    if(pivot - start < 2) {
        if(pivot - start == 1) {
            if(values[start] < values[pivot]) {
                swapIndex = indecies[start];
                swapValue = values[start];
                indecies[start] = indecies[pivot];
                values[start] = values[pivot];
                indecies[pivot] = swapIndex;
                values[pivot] = swapValue;
            }
        }
        return;
    }
    
    int i = start;
    int j = pivot - 1;
    
    while(i < j) {
        while(values[i] >= values[pivot] && i < j) {
            i++;
        }
        while(values[j] <= values[pivot] && j > i) {
            j--;
        }
        swapIndex = indecies[i];
        swapValue = values[i];
        indecies[i] = indecies[j];
        values[i] = values[j];
        indecies[j] = swapIndex;
        values[j] = swapValue;
    }
    if(i == pivot) {
        std::cout << "piiiiiiivor\n\n\n";
        std::cout << start << " -> " << i - 1 << "\n\n";
        quickSortIndeciesLargeToSmall(indecies, values, start, i - 1);
        return;
    }
    if(values[i] > values[pivot]) {
        i++;
    }
    swapIndex = indecies[i];
    swapValue = values[i];
    indecies[i] = indecies[pivot];
    values[i] = values[pivot];
    indecies[pivot] = swapIndex;
    values[pivot] = swapValue;
    
    //    printList(values, length);
    //    printList(indecies, length);
    //
    //    std::cout << start << " -> " << i - 1 << "\n";
    //    std::cout << i + 1 << " -> " << pivot << "\n\n";
    
    quickSortIndeciesLargeToSmall(indecies, values, start, i - 1);
    quickSortIndeciesLargeToSmall(indecies, values, i + 1, pivot);
}
