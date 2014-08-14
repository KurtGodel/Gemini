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
#include "HashTable.h"
#include "Evaluator.h"
#include "OpeningBook.h"
#include "RunAIInput.h"
#include "MoveConverter.h"
#include "Move.h"
#include "MovementCalculator.h"

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
    int maximumTime;
    float gameStage();
    std::vector < std::string > getPrincipleLine(int from, int to, bool isAIBlack);
    void getPrincipleLineTree(int layer, std::vector<std::string>* rtn, bool isAIBlack);
    void getPrincipleLineMakeMove(Move m, int layer, std::vector<std::string>* rtn, bool isAIBlack);
    
    float currentSearchDepth = 1;
    int startTime;
    int currentTime;
    RunAIInput* gameAiCommunicator;
    Evaluator evaluator;
    static const int maxNumberOfPly = 20;
    Move move[500*maxNumberOfPly];
    int moveLength[maxNumberOfPly];
    int killer[maxNumberOfPly][2];
    int moveOrder[500*maxNumberOfPly];
    float attemptedLateMoveReduction[maxNumberOfPly];
    float evalOrder[500*maxNumberOfPly];
    uint64_t parentZobrist[maxNumberOfPly];      // used to detect loops
    
    Board board;
    MoveConverter moveConverter;
    
    MovementCalculator movementCalculator;
    
    
    bool useOpeningBook = true;                 // passed 0.05 test
    
    
    
    bool useNullMove = true;                    // passed 0.05 test

    int minimumLegalMovesToNullMovePrune = 15;  // I refuse to tounament-test this, because the fact that I use material to determine wins means that
                                                // Zugzwang will be undervalued durings tournament-tests
    
    int minimumPiecesToNullMovePrune = 6;       // See above
    
    float nullR = 2;                            // passed 0.05 test compared to 1 and 3
    
    
    
    bool useLateMoveReductions = true;          // passed 0.05 test
    
    int minimumLegalMovesToLMR = 15;            // TODO
    
    int numberOfNonLateMoveReductions = 4;      // essentially tied with 3
    
    
    
    bool useFutilityPruning = true;             // passed 0.05 test
    
    int futilityMargin = 500;                   // passed 0.20 test compared to 1000
    
    
    
    
    
    float maxDepthSearch = 20;
    float depthStepSize = 1;
    int debugVar = 0;
    bool checkForCycle(int layer);
    
    int search(float depth);
    void setMoves(int layer);
    int aiInCheck(int king);
    int humanInCheck(int kingsq);
    bool willMovePutMeInCheck(int king, Move move);
    int futilityPruning(int layer, float depthLeft, int alpha, int beta, int endOfLastMove, Node* hash);
    
    int alphaBetaTree(int layer, float depthLeft, int alpha, int beta, int endOfLastMove);
    int alphaBetaMakeMove(Move m, int layer, float depthLeft, int alpha, int beta);
    void orderMoves(int layer, Node* hash, float depthLeft, int alpha, int beta);
    float normalMoveBasicOrderEval(Move m);
    
    Node* searchTranspositionTable();
    void addElementToTranspositionTable(int bestMove, int value, int betaSearchedAt, float depthSearched, bool searchCompleted);
    void addHashToTranspositionTable(uint64_t hash, int bestMove, int value, int betaSearchedAt, float depthSearched, bool searchCompleted);
    Node* searchTranspositionTableForHash(uint64_t hash);
    
    int evalPosition(int layer, int to, Node* node, int beta, float depthToSearch);
    float getExtension(int layer, Move m, float depthLeft, bool inCheck, int rank);
    float moveEval(Move m, int layer);
    float minimumDepthLeftForDetailedMoveOrdering = 100;            // moveEval()
    float minimumDepthLeftForSemiDetailedMoveOrdering = 100;        // Static Exchange Evaluation (SEE)
    float staticExchangeEvaluation(Move m, int layer);
    
    float moveOrderSearch(Move m, int layer, float depthToSearch);
    int numberOfPieceOnBoard(int piece);
    std::vector < uint64_t > duplicate;
    bool uintStringEqual(uint64_t num, std::string str);
    bool boardEquality(char* arr);
    bool debugFunction();
    
    int nodeCount = 0;
    
    void sortIndices(int*, float*, int length);
    
    void quickSortIndeciesLargeToSmall(int* indecies, float* values, int start, int pivot);
};

#endif /* defined(__Schach__AI__) */