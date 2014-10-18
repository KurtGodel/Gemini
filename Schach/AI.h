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
    void run();                                                     // this function is called by aiThread in main() in an infinite-loop
    OpeningBook openingBook;                                        // provides moves in opening book
private:
    float boardValue = 0;
    int cpuTimeLeft;
    int minimumTime;
    int maximumTime;
    
    float currentSearchDepth = 1;
    int startTime;                                                  // the time (in ms) a search started
    int currentTime;                                                // set to the current time (in ms) at the end of searches to keep track of how much time has been spent on searching this position
    RunAIInput* gameAiCommunicator;                                 // used to send information to and frm the user interface
    Evaluator evaluator;                                            // evaluates positions; it also keeps a running material count
    static const int maxNumberOfPly = 20;                           // the number of ply we have memory to search
    Move move[500*maxNumberOfPly];                                  // keeps track of all legal moves at each depth (each depth has 500 items it can fill, so if a position has more than 500 moves, this will cause problems - this should be impossible)
    int moveLength[maxNumberOfPly];                                 // because all moves are kept track of in a single array, this keeps track of how many legal moves each depth ahs
    int killer[maxNumberOfPly][2];                                  // keeps track of "killer" moves
    int moveOrder[500*maxNumberOfPly];                              // used to order moves (each element "points" to a move in move[])
    float attemptedLateMoveReduction[maxNumberOfPly];               // used to keep track of whether the momve being searched is under LMR
    float evalOrder[500*maxNumberOfPly];                            // used to order moves (each element is associated with a move in move[] and represents a guess at its importance of being searched)
    uint64_t parentZobrist[maxNumberOfPly];                         // an array of all parent's past zobrist-hashes; this is used to detect loops
    Board board;                                                    // current board
    MoveConverter moveConverter;
    MovementCalculator movementCalculator;
    
    // search options
    bool useOpeningBook = true;
    bool useNullMove = true;
    int minimumLegalMovesToNullMovePrune = 15;
    int minimumPiecesToNullMovePrune = 6;
    float nullR = 2;
    bool useLateMoveReductions = true;
    int minimumLegalMovesToLMR = 15;
    int numberOfNonLateMoveReductions = 4;
    float maxDepthSearch = 20;                                      // maximum depth a position can be searched
    float depthStepSize = 1;                                        // amount by which each iteration increases the depth searched
    
    float minimumDepthLeftForDetailedMoveOrdering = 100;            // moveEval()
    float minimumDepthLeftForSemiDetailedMoveOrdering = 100;        // Static Exchange Evaluation (SEE)
    std::vector < uint64_t > duplicate;                             // used to keep track of which positions will result in a three-move draw
    int nodeCount = 0;                                              // keeps track of the number of nodes searched
    
    /*
     "search" calls "alphaBetaMakeMove", which calls "alphaBetaTree", which calls "alphaBetaMakeMove", etc.
    */
    int search(float depth);        // begins search of moves to find the best one
    int alphaBetaTree(int layer, float depthLeft, int alpha, int beta, int endOfLastMove);      // loops through moves and finds best one (or cutt-off)
    int alphaBetaMakeMove(Move m, int layer, float depthLeft, int alpha, int beta);             // changes the position based on inputed move
    
    /*
     these functions are used to generate a list of legal moves
    */
    void setMoves(int layer);
    int aiInCheck(int king);
    int humanInCheck(int kingsq);
    bool willMovePutMeInCheck(int king, Move move);
    
    /*
     these functions interact with the transposition table
    */
    Node* searchTranspositionTable();                                                   // searches transposition table for current position
    void addElementToTranspositionTable(int bestMove, int value, int betaSearchedAt, float depthSearched, bool searchCompleted);        // adds current position and computed information about it to the transposition table
    void addHashToTranspositionTable(uint64_t hash, int bestMove, int value, int betaSearchedAt, float depthSearched, bool searchCompleted);        // adds current position and computed information about it to the transposition table; this is used if we have already precomputed the current position's Zobrist hash and is faster than addElementToTranspositionTable()
    Node* searchTranspositionTableForHash(uint64_t hash);                               // searches transposition table for the current position
    
    /*
     these functions are used to find the principle line
    */
    std::vector < std::string > getPrincipleLine(int from, int to, bool isAIBlack);     // returns the principle line
    void getPrincipleLineTree(int layer, std::vector<std::string>* rtn, bool isAIBlack);    // finds the next node in the principle line
    void getPrincipleLineMakeMove(Move m, int layer, std::vector<std::string>* rtn, bool isAIBlack);    // changes board, so that we can get the new position and the find the next move in the principle line
    
    
    /*
     these functions are used to sort moves before fully evaluating them in order to achieve alpha-beta cuttoffs quicker
    */
    void orderMoves(int layer, Node* hash, float depthLeft, int alpha, int beta);
    float moveEval(Move m, int layer);                                                  // quickly evaluates a move's potential for sorting
    float staticExchangeEvaluation(Move m, int layer);                                  // determines whether a piece is being moved to a safe square
    float moveOrderSearch(Move m, int layer, float depthToSearch);                      // searches a move to a shallower depth to order it better
    void sortIndices(int*, float*, int length);                                         // sorts moves based on their given potential values
    void quickSortIndeciesLargeToSmall(int* indecies, float* values, int start, int pivot); // sorts moves based on their given potential values using quick-sort
    float normalMoveBasicOrderEval(Move m);                                             // fast eval used for non-captures
    
    
    bool checkForCycle(int layer);                                                      // this allows us to catch 3-move draw rules and reduces the nodes we have to search by catching cycles of moves
    int evalPosition(int layer, int to, Node* node, int beta, float depthToSearch);     // evaluate's a position's value
    float getExtension(int layer, Move m, float depthLeft, bool inCheck, int rank);     // extends/reduces search depths
    
    
    bool boardEquality(char* arr);                                                      // determines if a board represented as an array of chars is equal to the current board
    float gameStage();      // returns a float between 0 and 1 representing the stage of the game (1 = beginning, 0 = end)
};

#endif /* defined(__Schach__AI__) */