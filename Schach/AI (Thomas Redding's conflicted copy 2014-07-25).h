//
//  AI.h
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__AI__
#define __Schach__AI__

#include <iostream>
#include <mutex>
#include "Board.h"
#include "Bitboard.h"
#include "HashTable.h"
#include "Evaluator.h"
#include "OpeningBook.h"
#include "RunAIInput.h"
#include "MoveConverter.h"

class AI
{
public:
    AI(RunAIInput* gameAiCommunicator);
    void run();
    OpeningBook openingBook;
private:
    float boardValue = 0;
    int cpuTimeLeft;
    int minimumTime;
    float gameStage();
    std::vector < std::string > getPrincipleLine(int from, int to, bool isAIBlack);
    void getPrincipleLineTree(int layer, std::vector<std::string>* rtn, bool isAIBlack);
    void getPrincipleLineMakeMove(int from, int to, int layer, std::vector<std::string>* rtn, bool isAIBlack);
    
    int debugVar = 0;
    
    float currentSearchDepth = 1;
    int startTime;
    int currentTime;
    RunAIInput* gameAiCommunicator;
    Evaluator evaluator;
    static const int maxNumberOfPly = 20;
    int move[500*maxNumberOfPly];
    int moveLength[maxNumberOfPly];
    int killer[maxNumberOfPly][2];
    int moveOrder[500*maxNumberOfPly];
    float evalOrder[500*maxNumberOfPly];
    
    Bitboard bitboard;
    Board board;
    MoveConverter moveConverter;
    
    int search(float depth);
    void setMoves(int layer);
    int aiInCheck(int king);
    int humanInCheck(int kingsq);
    bool willMovePutMeInCheck(int king, int from, int to);
    
    int alphaBetaTree(int layer, float depthLeft, int alpha, int beta, int endOfLastMove);
    int alphaBetaMakeMove(int from, int to, int layer, float depthLeft, int alpha, int beta);
    
    void orderMoves(int layer, Node* hash, float depthLeft, int alpha, int beta);
    Node* searchTranspositionTable();
    void addElementToTranspositionTable(int value, int bestMove, int betaCutoff, float depthSearched);
    int evalPosition(int layer, int to, Node* node, int beta, float depthToSearch);
    
    float checkExtension(float depthLeft);
    float captureExtension(float captureDiff, float depthLeft);
    float moveEval(int from, int to, int layer);
    float promotionDangerExtension(float depthLeft);
    float lateMoveReductionExtension(float depthLeft);
    
    float depthDiffForMoveOrdering = -3.0;
    float minimumDepthLeftForDetailedMoveOrdering = 3.0;
    
    float moveOrderSearch(int from, int to, int layer, float depthToSearch);
    int numberOfPieceOnBoard(int piece);
    void addHashToTranspositionTable(uint64_t hash, int value, int bestMove, int betaCutoff, float depthSearched);
    Node* searchTranspositionTableForHash(uint64_t hash);
    std::vector < uint64_t > duplicate;
};

#endif /* defined(__Schach__AI__) */